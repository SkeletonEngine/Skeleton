// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

namespace Skeleton::Vulkan {

// Forward declaration to avoid circular include between VulkanRenderer and VulkanMesh
class VulkanRenderer;

class VulkanMesh {
 public:
  VulkanMesh(const std::string& path, VulkanRenderer* renderer);
  ~VulkanMesh();

 public:
  void RecordRenderCommands(VkCommandBuffer command_buffer) const;

 private:
  VkBuffer      vertex_buffer_            = VK_NULL_HANDLE;
  VmaAllocation vertex_buffer_allocation_ = VK_NULL_HANDLE;
  VkBuffer      index_buffer_             = VK_NULL_HANDLE;
  VmaAllocation index_buffer_allocation_  = VK_NULL_HANDLE;
  uint32_t      index_count_              = 0;

  // Non-owning pointer to the renderer
  VulkanRenderer* renderer_;
};

}  // namespace Skeleton::Vulkan
