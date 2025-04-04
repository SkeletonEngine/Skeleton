// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

namespace Skeleton::Vulkan {

VkCommandBuffer VulkanRenderer::BeginSingleUseCommandBuffer() {
  // Allocate the command buffer
  VkCommandBufferAllocateInfo command_buffer_alloc_info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
  command_buffer_alloc_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  command_buffer_alloc_info.commandPool        = command_pool_;
  command_buffer_alloc_info.commandBufferCount = 1;

  VkCommandBuffer command_buffer;
  vkAllocateCommandBuffers(device_, &command_buffer_alloc_info, &command_buffer);

  // Begin recording commands to the command buffer
  VkCommandBufferBeginInfo begin_info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vkBeginCommandBuffer(command_buffer, &begin_info);

  return command_buffer;
}

void VulkanRenderer::EndSingleUseCommandBuffer(VkCommandBuffer command_buffer) {
  // End recording commands
  vkEndCommandBuffer(command_buffer);

  // Submit the command buffer
  VkSubmitInfo submit_info { VK_STRUCTURE_TYPE_SUBMIT_INFO };
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers    = &command_buffer;
  vkQueueSubmit(graphics_queue_, 1, &submit_info, VK_NULL_HANDLE);
}

}  // namespace Skeleton::Vulkan
