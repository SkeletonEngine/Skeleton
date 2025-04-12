// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

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
  CreateSwapchain();
  CreateSwapchainImageViews();
  CreateRenderPass();
  CreateGraphicsPipeline();
  CreateSwapchainFramebuffers();
  CreateCommandPool();
  CreateRenderCommandBuffer();
  CreateSyncObjects();
  CreateVmaAllocator();
  CreateMesh();
  CreateUniformBuffers();

  // Register a callback so that we are notified when the client window is resized
  // When that happens, we will need to recreate the swapchain
  window->RegisterFramebufferSizeCallback([&](int width, int height) {
    window_framebuffer_resized_ = true;
    window_minimized_ = (width == 0 || height == 0);
  });
}

VulkanRenderer::~VulkanRenderer() {
  vkDeviceWaitIdle(device_);

  DestroyUniformBuffers();
  DestroyMesh();
  DestroyVmaAllocator();
  DestroySyncObjects();
  DestroyRenderCommandBuffer();
  DestroyCommandPool();
  DestroySwapchainFramebuffers();
  DestroyGraphicsPipeline();
  DestroyRenderPass();
  DestroySwapchainImageViews();
  DestroySwapchain();
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
  VK_CHECK(image_acquire_result);

  // Once we know that we have an image to render to, we can reset the fence for the current frame
  vkResetFences(device_, 1, &in_flight_fences_[current_frame]);

  // Record the render commands to a command buffer
  vkResetCommandBuffer(render_command_buffers_[current_frame], 0);
  RecordRenderCommandBuffer(render_command_buffers_[current_frame], image_index);

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
