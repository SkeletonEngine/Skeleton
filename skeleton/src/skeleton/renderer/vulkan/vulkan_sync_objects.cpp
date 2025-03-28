// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <volk.h>
#include "skeleton/renderer/vulkan/vulkan_check.hpp"

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateSyncObjects() {
  /* There's nothing special we need to do for the semaphores */
  VkSemaphoreCreateInfo semaphore_info { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
  VK_CHECK(vkCreateSemaphore(device_, &semaphore_info, allocator_, &image_available_semaphore_));
  VK_CHECK(vkCreateSemaphore(device_, &semaphore_info, allocator_, &render_complete_semaphore_));

  /* We create the fence in a signalled state so that we don't deadlock waiting for the first image */
  VkFenceCreateInfo fence_info { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
  fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  VK_CHECK(vkCreateFence(device_, &fence_info, allocator_, &in_flight_fence_));
}

void VulkanRenderer::DestroySyncObjects() {
  vkDestroyFence(device_, in_flight_fence_, allocator_);
  vkDestroySemaphore(device_, image_available_semaphore_, allocator_);
  vkDestroySemaphore(device_, render_complete_semaphore_, allocator_);
}

}  // namespace Skeleton::Vulkan
