// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/renderer/vulkan/vulkan_editor_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <imgui/backends/imgui_impl_vulkan.h>

namespace Skeleton::Vulkan {

void VulkanEditorRenderer::CreateImguiDescriptorPool() {
  // Imgui needs its own descriptor pool
  // The pool needs to be big enough to allocate the minimum descriptors for Imgui to work internally, plus one for
  // each framebuffer we will be blitting to an Imgui window as an image
  uint32_t descriptor_count = IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE
                            + static_cast<uint32_t>(swapchain_images_.size());
  VkDescriptorPoolSize pool_sizes[] = {
    { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptor_count },
  };
  VkDescriptorPoolCreateInfo pool_info = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
  pool_info.flags   = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  pool_info.maxSets = 0;
  for (VkDescriptorPoolSize& pool_size : pool_sizes) {
    pool_info.maxSets += pool_size.descriptorCount;
  }
  pool_info.poolSizeCount = static_cast<uint32_t>(IM_ARRAYSIZE(pool_sizes));
  pool_info.pPoolSizes    = pool_sizes;
  VK_CHECK(vkCreateDescriptorPool(device_, &pool_info, allocator_, &imgui_descriptor_pool_));
}

void VulkanEditorRenderer::DestroyImguiDescriptorPool() {
  vkDestroyDescriptorPool(device_, imgui_descriptor_pool_, allocator_);
}

}  // namespace Skeleton::Vulkan
