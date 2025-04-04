// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

#include <vector>

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateVertexBuffer() {
  const std::vector<float> vertices = {
     0.0f, -0.5f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
  };

  vertex_buffer_vertex_count_ = vertices.size();

  // Create staging buffer in host-visible memory
  VkBufferCreateInfo staging_buffer_info { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
  staging_buffer_info.size        = vertex_buffer_vertex_count_ * sizeof(float);
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

  // Map the staging buffer's allocated memory and upload vertex data to it
  void* mapped_memory;
  vmaMapMemory(vma_allocator_, staging_allocation, &mapped_memory);
  std::memcpy(mapped_memory, vertices.data(), vertex_buffer_vertex_count_ * sizeof(float));
  vmaUnmapMemory(vma_allocator_, staging_allocation);

  // Create vertex buffer in device-local memory
  VkBufferCreateInfo vertex_buffer_info { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
  vertex_buffer_info.size        = vertex_buffer_vertex_count_ * sizeof(float);
  vertex_buffer_info.usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
  vertex_buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VmaAllocationCreateInfo vertex_alloc_info { };
  vertex_alloc_info.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  vertex_alloc_info.usage         = VMA_MEMORY_USAGE_AUTO;
  vertex_alloc_info.flags         = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

  VK_CHECK(vmaCreateBuffer(vma_allocator_, &vertex_buffer_info, &vertex_alloc_info,
                           &vertex_buffer_, &vertex_buffer_allocation_, nullptr));

  // Use a transfer operation to copy the contents of the staging buffer into the vertex buffer
  // Allocate a command buffer to use for the transfer operation
  VkCommandBufferAllocateInfo command_buffer_alloc_info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
  command_buffer_alloc_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  command_buffer_alloc_info.commandPool        = command_pool_;
  command_buffer_alloc_info.commandBufferCount = 1;

  VkCommandBuffer transfer_command_buffer;
  vkAllocateCommandBuffers(device_, &command_buffer_alloc_info, &transfer_command_buffer);

  // Begin recording commands to the command buffer
  VkCommandBufferBeginInfo begin_info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vkBeginCommandBuffer(transfer_command_buffer, &begin_info);
  
  // Record copy command
  VkBufferCopy copy_region { };
  copy_region.size = vertex_buffer_vertex_count_ * sizeof(float);
  vkCmdCopyBuffer(transfer_command_buffer, staging_buffer, vertex_buffer_, 1, &copy_region);

  // End recording commands
  vkEndCommandBuffer(transfer_command_buffer);

  // Submit the command buffer
  VkSubmitInfo submit_info { VK_STRUCTURE_TYPE_SUBMIT_INFO };
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers    = &transfer_command_buffer;
  vkQueueSubmit(graphics_queue_, 1, &submit_info, VK_NULL_HANDLE);

  // Wait for the transfer command to complete
  vkDeviceWaitIdle(device_);

  // Destroy the command buffer
  vkFreeCommandBuffers(device_, command_pool_, 1, &transfer_command_buffer);

  // Destroy the staging buffer
  vmaDestroyBuffer(vma_allocator_, staging_buffer, staging_allocation);
}

void VulkanRenderer::DestroyVertexBuffer() {
  vmaDestroyBuffer(vma_allocator_, vertex_buffer_, vertex_buffer_allocation_);
}

}  // namespace Skeleton::Vulkan
