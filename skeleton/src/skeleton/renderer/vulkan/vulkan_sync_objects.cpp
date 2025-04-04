// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateSyncObjects() {
  /* There's nothing special we need to do for the semaphores */
  image_available_semaphores_.resize(kMaxFramesInFlight);
  render_complete_semaphores_.resize(kMaxFramesInFlight);
  VkSemaphoreCreateInfo semaphore_info { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    VK_CHECK(vkCreateSemaphore(device_, &semaphore_info, allocator_, &image_available_semaphores_[i]));
    VK_CHECK(vkCreateSemaphore(device_, &semaphore_info, allocator_, &render_complete_semaphores_[i]));
  }

  /* We create the fence in a signalled state so that we don't deadlock waiting for the first image */
  in_flight_fences_.resize(kMaxFramesInFlight);
  VkFenceCreateInfo fence_info { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
  fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    VK_CHECK(vkCreateFence(device_, &fence_info, allocator_, &in_flight_fences_[i]));
  }
}

void VulkanRenderer::DestroySyncObjects() {
  for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
    vkDestroyFence(device_, in_flight_fences_[i], allocator_);
    vkDestroySemaphore(device_, image_available_semaphores_[i], allocator_);
    vkDestroySemaphore(device_, render_complete_semaphores_[i], allocator_);
  }
}

}  // namespace Skeleton::Vulkan
