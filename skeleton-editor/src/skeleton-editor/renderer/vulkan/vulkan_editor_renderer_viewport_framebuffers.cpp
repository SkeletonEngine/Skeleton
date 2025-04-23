// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/renderer/vulkan/vulkan_editor_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <imgui/backends/imgui_impl_vulkan.h>

namespace Skeleton::Vulkan {

void VulkanEditorRenderer::CreateViewportFramebuffer(size_t i) {
  // Create image to use as render target
  VkImageCreateInfo image_info { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
  image_info.imageType   = VK_IMAGE_TYPE_2D;
  image_info.format      = swapchain_image_format_;
  image_info.extent      = { editor_viewport_extent_.width, editor_viewport_extent_.height, 1 };
  image_info.mipLevels   = 1;
  image_info.arrayLayers = 1;
  image_info.samples     = VK_SAMPLE_COUNT_1_BIT;
  image_info.tiling      = VK_IMAGE_TILING_OPTIMAL;
  image_info.usage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
  image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  vkCreateImage(device_, &image_info, allocator_, &editor_viewport_images_[i]);

  // Allocate image memory
  VkMemoryRequirements mem_reqs;
  vkGetImageMemoryRequirements(device_, editor_viewport_images_[i], &mem_reqs);
  VmaAllocationCreateInfo alloc_info = { };
  alloc_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
  alloc_info.preferredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  VK_CHECK(vmaAllocateMemory(vma_allocator_, &mem_reqs, &alloc_info, &editor_viewport_image_allocations_[i], nullptr));
  VK_CHECK(vmaBindImageMemory(vma_allocator_, editor_viewport_image_allocations_[i], editor_viewport_images_[i]));

  // Create image view
  VkImageViewCreateInfo image_view_info { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
  image_view_info.image = editor_viewport_images_[i];
  image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  image_view_info.format = swapchain_image_format_;
  image_view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  image_view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  image_view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  image_view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  image_view_info.subresourceRange.baseMipLevel = 0;
  image_view_info.subresourceRange.levelCount = 1;
  image_view_info.subresourceRange.baseArrayLayer = 0;
  image_view_info.subresourceRange.layerCount = 1;
  VK_CHECK(vkCreateImageView(device_, &image_view_info, allocator_, &editor_viewport_image_views_[i]));

  // Create sampler
  VkSamplerCreateInfo sampler_info { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
  sampler_info.magFilter     = VK_FILTER_LINEAR;
  sampler_info.minFilter     = VK_FILTER_LINEAR;
  sampler_info.mipmapMode    = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  sampler_info.addressModeU  = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  sampler_info.addressModeV  = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  sampler_info.addressModeW  = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  sampler_info.minLod        = -1000;
  sampler_info.maxLod        = 1000;
  sampler_info.maxAnisotropy = 1.0f;
  VK_CHECK(vkCreateSampler(device_, &sampler_info, allocator_, &editor_viewport_samplers_[i]));

  // Allocate descriptor set - these are the handles Imgui will use to reference the image views
  editor_viewport_descriptor_sets_[i] = ImGui_ImplVulkan_AddTexture(editor_viewport_samplers_[i],
                                                                    editor_viewport_image_views_[i],
                                                                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

  // Create framebuffer to be used as render target
  VkImageView attachments[] = {
    editor_viewport_image_views_[i]
  };
  VkFramebufferCreateInfo framebuffer_info { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
  framebuffer_info.renderPass      = render_pass_;
  framebuffer_info.attachmentCount = 1;
  framebuffer_info.pAttachments    = attachments;
  framebuffer_info.width           = editor_viewport_extent_.width;
  framebuffer_info.height          = editor_viewport_extent_.height;
  framebuffer_info.layers          = 1;

  VK_CHECK(vkCreateFramebuffer(device_, &framebuffer_info, allocator_, &editor_viewport_framebuffers_[i]));
}

void VulkanEditorRenderer::DestroyViewportFramebuffer(size_t i) {
  vkDestroyFramebuffer(device_, editor_viewport_framebuffers_[i], allocator_);
  ImGui_ImplVulkan_RemoveTexture(editor_viewport_descriptor_sets_[i]);
  vkDestroySampler(device_, editor_viewport_samplers_[i], allocator_);
  vkDestroyImageView(device_, editor_viewport_image_views_[i], allocator_);
  vmaFreeMemory(vma_allocator_, editor_viewport_image_allocations_[i]);
  vkDestroyImage(device_, editor_viewport_images_[i], allocator_);
}

}  // namespace Skeleton::Vulkan
