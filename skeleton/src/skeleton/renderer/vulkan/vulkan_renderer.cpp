// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

#include <chrono>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Skeleton::Vulkan {

VulkanRenderer::VulkanRenderer(const ApplicationSettings& settings, Window* window)
: window_(window), vsync_(settings.renderer.vsync) {
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
  window->RegisterFramebufferSizeCallback([&](int width, int height) {
    window_framebuffer_resized_ = true;
    window_minimized_ = (width == 0 || height == 0);
  });

  
  // TODO(jack): remove test code
  // Upload identity matrix to all uniform buffers
  std::vector<glm::mat4> identity_matrices(3, glm::mat4(1.0f));

  for (const auto& ub : uniform_buffers_) {
    for (uint32_t i = 0; i < kMaxFramesInFlight; ++i) {
      std::memcpy(ub.second.mapped_memory[i], identity_matrices.data(), sizeof(glm::mat4) * identity_matrices.size());
    }
  }
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

  // If the window was resized, recreate the swapchain
  if (window_framebuffer_resized_) {
    window_framebuffer_resized_ = false;
    RecreateSwapchain();
  }

  // Keeps track of which set of command buffers/sync objects to use
  static uint32_t current_frame = 0;
  current_frame = (current_frame + 1) % kMaxFramesInFlight;
  
  // TODO(jack): remove test code
  // Upload rotation matrix
  static auto start_time = std::chrono::high_resolution_clock::now();
  auto current_time = std::chrono::high_resolution_clock::now();
  float angle = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();
  glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));

  for (const auto& ub : uniform_buffers_) {
    std::memcpy(ub.second.mapped_memory[current_frame], glm::value_ptr(rotation_matrix), sizeof(glm::mat4));
  }

  // Wait for the previous frame to finish, if necessary
  vkWaitForFences(device_, 1, &in_flight_fences_[current_frame], VK_TRUE, UINT64_MAX);

  // Acquire an image from the swapchain
  uint32_t image_index;
  VkResult image_acquire_result = vkAcquireNextImageKHR(
    device_, swapchain_, UINT64_MAX, image_available_semaphores_[current_frame], VK_NULL_HANDLE, &image_index);

  // If the swapchain is out of date, we need to recreate it
  if (image_acquire_result == VK_ERROR_OUT_OF_DATE_KHR) {
    RecreateSwapchain();
    return;
  }
#ifdef SK_BUILD_DEBUG
  VK_CHECK(image_acquire_result);
#endif  // SK_BUILD_DEBUG
  
  // Once we know that we have an image to render to, we can reset the fence for the current frame
  vkResetFences(device_, 1, &in_flight_fences_[current_frame]);

  // Record the render commands to a command buffer
  vkResetCommandBuffer(render_command_buffers_[current_frame], 0);
  RecordRenderCommandBuffer(render_command_buffers_[current_frame], image_index, current_frame);

  // Submit the command buffer
  VkSubmitInfo submit_info { VK_STRUCTURE_TYPE_SUBMIT_INFO };
  VkSemaphore wait_semaphores[]   = { image_available_semaphores_[current_frame] };
  VkSemaphore signal_semaphores[] = { render_complete_semaphores_[current_frame] };
  VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores    = wait_semaphores;
  submit_info.pWaitDstStageMask  = wait_stages;
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores  = signal_semaphores;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers    = &render_command_buffers_[current_frame];
  VK_CHECK(vkQueueSubmit(graphics_queue_, 1, &submit_info, in_flight_fences_[current_frame]));

  // Present the frame
  VkPresentInfoKHR present_info { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores    = signal_semaphores;
  VkSwapchainKHR swapchains[] = { swapchain_ };
  present_info.swapchainCount     = 1;
  present_info.pSwapchains        = swapchains;
  present_info.pImageIndices      = &image_index;
  vkQueuePresentKHR(present_queue_, &present_info);
}

}  // namespace Skeleton::Vulkan
