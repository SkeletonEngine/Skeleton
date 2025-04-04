// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateVertexBuffer() {
  const std::vector<float> vertices = {
     0.0f, -0.5f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
  };

  vertex_buffer_vertex_count_ = vertices.size();

	VkBufferCreateInfo buffer_info { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	buffer_info.size        = vertices.size() * sizeof(float);
	buffer_info.usage       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VmaAllocationCreateInfo alloc_info { };
  alloc_info.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
  alloc_info.usage         = VMA_MEMORY_USAGE_AUTO;
  alloc_info.flags         = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

  VmaAllocationInfo allocation_info;
  VK_CHECK(vmaCreateBuffer(vma_allocator_, &buffer_info, &alloc_info, &vertex_buffer_, &vertex_buffer_allocation_, &allocation_info));

  // Map the allocated memory and upload the vertex data to the GPU
  void* mapped_memory;
  vmaMapMemory(vma_allocator_, vertex_buffer_allocation_, &mapped_memory);
  std::memcpy(mapped_memory, vertices.data(), buffer_info.size);
  vmaUnmapMemory(vma_allocator_, vertex_buffer_allocation_);
}

void VulkanRenderer::DestroyVertexBuffer() {
  vmaDestroyBuffer(vma_allocator_, vertex_buffer_, vertex_buffer_allocation_);
}

}  // namespace Skeleton::Vulkan
