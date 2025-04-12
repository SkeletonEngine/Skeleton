// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

#include "skeleton/renderer/common/shader_reflection/shader_reflection_details.hpp"
#include "skeleton/renderer/common/spv_file.hpp"

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateUniformBuffers() {
  const char* kVertPath = "build/shaders/test.vert.spv";
  std::vector<uint32_t> vert_spv = ReadSpvFile(kVertPath);
  ShaderReflectionDetails vert_reflection(vert_spv);

  ShaderBufferLayout u_mvp_layout = vert_reflection.uniform_buffers["MvpUniformBuffer"];

  u_mvp_buffers_.resize(kMaxFramesInFlight);
  u_mvp_allocations_.resize(kMaxFramesInFlight);
  u_mvp_mapped_memory_.resize(kMaxFramesInFlight);

  VkBufferCreateInfo buffer_info { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
  buffer_info.size        = u_mvp_layout.GetSize();
  buffer_info.usage       = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  
  VmaAllocationCreateInfo alloc_create_info { };
  alloc_create_info.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
  alloc_create_info.usage = VMA_MEMORY_USAGE_AUTO;
  alloc_create_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;

  VmaAllocationInfo alloc_info;

  for (uint32_t i = 0; i < kMaxFramesInFlight; ++i) {
    VK_CHECK(vmaCreateBuffer(vma_allocator_, &buffer_info, &alloc_create_info, &u_mvp_buffers_[i], &u_mvp_allocations_[i], &alloc_info));
    u_mvp_mapped_memory_[i] = alloc_info.pMappedData;
  }
}

void VulkanRenderer::DestroyUniformBuffers() {
  for (uint32_t i = 0; i < kMaxFramesInFlight; ++i) {
    vmaDestroyBuffer(vma_allocator_, u_mvp_buffers_[i], u_mvp_allocations_[i]);
  }
}

}  // namespace Skeleton::Vulkan
