// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/core/core.hpp"

#include "skeleton/renderer/vulkan/vulkan_check.hpp"
#include "skeleton/renderer/vulkan/vulkan_device_queue_families.hpp"

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateCommandPool() {
  DeviceQueueFamilies queue_families(physical_device_, surface_);

  VkCommandPoolCreateInfo pool_info { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
  pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  pool_info.queueFamilyIndex = queue_families.GraphicsFamilyIndex();

  VK_CHECK(vkCreateCommandPool(device_, &pool_info, allocator_, &command_pool_));
}

void VulkanRenderer::DestroyCommandPool() {
  vkDestroyCommandPool(device_, command_pool_, allocator_);
}

}  // namespace Skeleton::Vulkan
