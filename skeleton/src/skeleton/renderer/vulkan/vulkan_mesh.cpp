// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_mesh.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include <vector>

namespace Skeleton::Vulkan {

VulkanMesh::VulkanMesh(const std::string& path, VulkanRenderer* renderer) : renderer_(renderer) {
  // Create vertex buffer
  const std::vector<float> vertices = {
    -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
  };
  renderer->CreateDeviceLocalBuffer(vertices.data(), vertices.size() * sizeof(float), &vertex_buffer_,
                                    &vertex_buffer_allocation_, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

  // Create index buffer
  const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
  };
  renderer->CreateDeviceLocalBuffer(indices.data(), indices.size() * sizeof(uint16_t), &index_buffer_,
                                    &index_buffer_allocation_, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

  // Store the index count for use when performing the draw operation later
  index_count_ = static_cast<uint32_t>(indices.size());
}

VulkanMesh::~VulkanMesh() {
  renderer_->DestroyBuffer(index_buffer_,  index_buffer_allocation_ );
  renderer_->DestroyBuffer(vertex_buffer_, vertex_buffer_allocation_);
}

void VulkanMesh::RecordRenderCommands(VkCommandBuffer command_buffer) const {
  // Bind the vertex buffer
  VkBuffer vertex_buffers[] = { vertex_buffer_ };
  VkDeviceSize offsets[] = { 0 };
  vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);

  // Bind the index buffer
  vkCmdBindIndexBuffer(command_buffer, index_buffer_, 0, VK_INDEX_TYPE_UINT16);

  // Issue draw command
  vkCmdDrawIndexed(command_buffer, index_count_, 1, 0, 0, 0);
}

}  // namespace Skeleton::Vulkan
