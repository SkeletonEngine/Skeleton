// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

namespace Skeleton::Vulkan {

// Creates a device-local buffer (usually a vertex or index buffer) and uploads arbitrary data to it
void VulkanRenderer::CreateDeviceLocalBuffer(const void* data, VkDeviceSize size, VkBuffer* buffer,
                                             VmaAllocation* allocation, VkBufferUsageFlagBits usage) {
  // Create staging buffer in host-visible memory
  VkBufferCreateInfo staging_buffer_info { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
  staging_buffer_info.size        = size;
  staging_buffer_info.usage       = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
  staging_buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VmaAllocationCreateInfo staging_alloc_info { };
  staging_alloc_info.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
  staging_alloc_info.usage         = VMA_MEMORY_USAGE_AUTO;
  staging_alloc_info.flags         = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

  VkBuffer staging_buffer;
  VmaAllocation staging_allocation;
  VK_CHECK(vmaCreateBuffer(vma_allocator_, &staging_buffer_info, &staging_alloc_info,
                           &staging_buffer, &staging_allocation, nullptr));

  // Map the staging buffer's allocated memory and upload data to it
  void* mapped_memory;
  vmaMapMemory(vma_allocator_, staging_allocation, &mapped_memory);
  std::memcpy(mapped_memory, data, size);
  vmaUnmapMemory(vma_allocator_, staging_allocation);

  // Create buffer in device-local memory
  VkBufferCreateInfo buffer_info { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
  buffer_info.size        = size;
  buffer_info.usage       = VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage;
  buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VmaAllocationCreateInfo alloc_info { };
  alloc_info.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  alloc_info.usage         = VMA_MEMORY_USAGE_AUTO;
  alloc_info.flags         = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

  VK_CHECK(vmaCreateBuffer(vma_allocator_, &buffer_info, &alloc_info, buffer, allocation, nullptr));

  // Use a transfer operation to copy the contents of the staging buffer into the buffer
  VkCommandBuffer transfer_command_buffer = BeginSingleUseCommandBuffer();

  // Record copy command
  VkBufferCopy copy_region { };
  copy_region.size = size;
  vkCmdCopyBuffer(transfer_command_buffer, staging_buffer, *buffer, 1, &copy_region);

  // End and submit the command buffer
  EndSingleUseCommandBuffer(transfer_command_buffer);

  // Wait for the transfer command to complete
  vkDeviceWaitIdle(device_);

  // Destroy the command buffer
  vkFreeCommandBuffers(device_, command_pool_, 1, &transfer_command_buffer);

  // Destroy the staging buffer
  vmaDestroyBuffer(vma_allocator_, staging_buffer, staging_allocation);
}

void VulkanRenderer::DestroyBuffer(VkBuffer buffer, VmaAllocation allocation) {
  vmaDestroyBuffer(vma_allocator_, buffer, allocation);
}

}  // namespace Skeleton::Vulkan
