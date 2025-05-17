// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

#include <chrono>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "skeleton/scene/components/components.hpp"
#include "skeleton/scene/scene.hpp"

namespace Skeleton::Vulkan {

VulkanRenderer::VulkanRenderer(const RendererSettings& settings, VkImageLayout final_layout)
    : window_(settings.window), projection_matrix_dirty_(kMaxFramesInFlight, true), final_image_layout_(final_layout),
      vsync_(settings.vsync) {
  CreateInstance();
#ifdef SK_BUILD_DEBUG
  CreateDebugMessenger();
#endif  // SK_BUILD_DEBUG
  CreateWindowSurface();
  ChoosePhysicalDevice();
  CreateDevice();
  CreateVmaAllocator();
  CreateSwapchain();
  CreateSwapchainImageViews();
  CreateRenderPass();
  CreateGraphicsPipeline();
  CreateSwapchainFramebuffers();
  CreateCommandPool();
  CreateRenderCommandBuffer();
  CreateSyncObjects();
  CreateMesh();

  // Register a callback so that we are notified when the client window is resized
  // When that happens, we will need to recreate the swapchain
  // We also need to recalculate the projection matrix in case the aspect ratio of the window has changed
  auto on_window_resize = [&](int width, int height) {
    window_framebuffer_resized_ = true;
    window_minimized_ = (width == 0 || height == 0);

    // Mark all camera matrices as dirty so they will be updated
    if (!window_minimized_) {
      for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
        projection_matrix_dirty_[i] = true;
      }
    }
  };

  settings.window->RegisterFramebufferSizeCallback(on_window_resize);
}

VulkanRenderer::~VulkanRenderer() {
  vkDeviceWaitIdle(device_);

  DestroyMesh();
  DestroySyncObjects();
  DestroyRenderCommandBuffer();
  DestroyCommandPool();
  DestroySwapchainFramebuffers();
  DestroyGraphicsPipeline();
  DestroyRenderPass();
  DestroySwapchainImageViews();
  DestroySwapchain();
  DestroyVmaAllocator();
  DestroyDevice();
  DestroyWindowSurface();
#ifdef SK_BUILD_DEBUG
  DestroyDebugMessenger();
#endif  // SK_BUILD_DEBUG
  DestroyInstance();
}

void VulkanRenderer::RenderFrame() {
  // If the window was minimized, don't render anything
  if (window_minimized_) {
    return;
  }

  BeginFrame();
  BeginRenderCommandBuffer();
  PerformSceneRenderPass();
  EndRenderCommandBuffer();
  EndFrame();
}

void VulkanRenderer::BeginFrame() {
  // If the window was resized, recreate the swapchain
  if (window_framebuffer_resized_) {
    window_framebuffer_resized_ = false;
    RecreateSwapchain();
  }

  // Keeps track of which set of command buffers/sync objects to use
  current_frame_ = (current_frame_ + 1) % kMaxFramesInFlight;

  // Wait for the previous frame to finish, if necessary
  vkWaitForFences(device_, 1, &in_flight_fences_[current_frame_], VK_TRUE, UINT64_MAX);

  // TODO(jack): remove test code
  // Upload rotation matrix
  static auto start_time = std::chrono::high_resolution_clock::now();
  auto current_time = std::chrono::high_resolution_clock::now();
  float angle = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();
  glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));
  std::memcpy(uniform_buffers_[kUboBindingModelMatrix].mapped_memory[current_frame_],
              glm::value_ptr(rotation_matrix), sizeof(glm::mat4));

  // Check the current camera in the ECS. If its fov/aspect/near/far have been altered, it should have been marked with
  // projection_matrix_dirty = true. If so, we will need to recalculate the projection matrix.
  entt::entity camera = scene_->get<CurrentCameraComponent>(root_).current_camera;
  CameraComponent& camera_component = scene_->get<CameraComponent>(camera);
  if (camera_component.projection_matrix_dirty) {
    for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
      projection_matrix_dirty_[i] = true;
    }
    camera_component.projection_matrix_dirty = false;
  }

  // TODO(jack): remove test code
  // If the camera matrix is dirty for the current frame, calculate it and upload it
  if (projection_matrix_dirty_[current_frame_]) {
    glm::mat4 projection_matrix = glm::perspective(camera_component.fov,
      static_cast<float>(render_target_extent_->width) / static_cast<float>(render_target_extent_->height),
      camera_component.clip_near, camera_component.clip_far);
    glm::mat4 view_matrix       = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    glm::mat4 camera_matrix = projection_matrix * view_matrix;
    std::memcpy(uniform_buffers_[kUboBindingCameraMatrix].mapped_memory[current_frame_],
                glm::value_ptr(camera_matrix), sizeof(glm::mat4));
    projection_matrix_dirty_[current_frame_] = false;
  }

  // Acquire an image from the swapchain
  VkResult image_acquire_result = vkAcquireNextImageKHR(
    device_, swapchain_, UINT64_MAX, image_available_semaphores_[current_frame_], VK_NULL_HANDLE, &image_index_);

  // If the swapchain is out of date, we need to recreate it
  if (image_acquire_result == VK_ERROR_OUT_OF_DATE_KHR) {
    RecreateSwapchain();
    return;
  }
#ifdef SK_BUILD_DEBUG
  VK_CHECK(image_acquire_result);
#endif  // SK_BUILD_DEBUG

  // Once we know that we have an image to render to, we can reset the fence for the current frame
  vkResetFences(device_, 1, &in_flight_fences_[current_frame_]);
}

void VulkanRenderer::EndFrame() {
  // Submit the command buffer
  VkSubmitInfo submit_info { VK_STRUCTURE_TYPE_SUBMIT_INFO };
  VkSemaphore wait_semaphores[]   = { image_available_semaphores_[current_frame_] };
  VkSemaphore signal_semaphores[] = { render_complete_semaphores_[current_frame_] };
  VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores    = wait_semaphores;
  submit_info.pWaitDstStageMask  = wait_stages;
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores  = signal_semaphores;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers    = &render_command_buffers_[current_frame_];
  VK_CHECK(vkQueueSubmit(graphics_queue_, 1, &submit_info, in_flight_fences_[current_frame_]));

  // Present the frame
  VkPresentInfoKHR present_info { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores    = signal_semaphores;
  VkSwapchainKHR swapchains[] = { swapchain_ };
  present_info.swapchainCount     = 1;
  present_info.pSwapchains        = swapchains;
  present_info.pImageIndices      = &image_index_;
  vkQueuePresentKHR(present_queue_, &present_info);
}

void VulkanRenderer::SetScene(entt::registry* scene) {
  scene_ = scene;
  root_  = GetRootNode(scene);
}

}  // namespace Skeleton::Vulkan
