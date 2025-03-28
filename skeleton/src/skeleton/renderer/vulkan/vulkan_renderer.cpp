// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"

#include "skeleton/renderer/vulkan/vulkan_check.hpp"

namespace Skeleton::Vulkan {

VulkanRenderer::VulkanRenderer(const ApplicationSettings& settings, Window* window)
    : window_(window), vsync_(settings.renderer.vsync) {
  CreateInstance();
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
}

VulkanRenderer::~VulkanRenderer() {
  vkDeviceWaitIdle(device_);

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
  DestroyInstance();
}

void VulkanRenderer::RenderFrame() {
  /* Wait for the previous frame to finish, if necessary */
  vkWaitForFences(device_, 1, &in_flight_fence_, VK_TRUE, UINT64_MAX);
  vkResetFences(device_, 1, &in_flight_fence_);

  /* Acquire an image from the swapchain */
  uint32_t image_index;
  vkAcquireNextImageKHR(device_, swapchain_, UINT64_MAX, image_available_semaphore_, VK_NULL_HANDLE, &image_index);

  /* Record the render commands to a command buffer */
  vkResetCommandBuffer(render_command_buffer_, 0);
  RecordRenderCommandBuffer(render_command_buffer_, image_index);

  /* Submit the command buffer */
  VkSubmitInfo submit_info { VK_STRUCTURE_TYPE_SUBMIT_INFO };
  VkSemaphore wait_semaphores[]   = { image_available_semaphore_ };
  VkSemaphore signal_semaphores[] = { render_complete_semaphore_ };
  VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores    = wait_semaphores;
  submit_info.pWaitDstStageMask  = wait_stages;
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores  = signal_semaphores;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers    = &render_command_buffer_;
  VK_CHECK(vkQueueSubmit(graphics_queue_, 1, &submit_info, in_flight_fence_));

  /* Presentation */
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
