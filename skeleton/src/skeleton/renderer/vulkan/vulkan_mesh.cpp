// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

#include <vector>

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateMesh() {
  // Create vertex buffer
  const std::vector<float> vertices = {
    -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
  };
  CreateDeviceLocalBuffer(vertices.data(), vertices.size() * sizeof(float), &vertex_buffer_,
                          &vertex_buffer_allocation_, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

  // Create index buffer
  const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
  };
  CreateDeviceLocalBuffer(indices.data(), indices.size() * sizeof(uint16_t), &index_buffer_,
                          &index_buffer_allocation_, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

  // Store the index count for performing the draw operation later
  index_count_ = static_cast<uint32_t>(indices.size());
}

void VulkanRenderer::DestroyMesh() {
  vmaDestroyBuffer(vma_allocator_, index_buffer_,  index_buffer_allocation_);
  vmaDestroyBuffer(vma_allocator_, vertex_buffer_, vertex_buffer_allocation_);
}

}  // namespace Skeleton::Vulkan
