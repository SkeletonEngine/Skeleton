// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <volk.h>
#include "skeleton/renderer/vulkan/vulkan_check.hpp"

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateSwapchainFramebuffers() {
  /* Create one framebuffer for each swapchain image view */
  swapchain_framebuffers_.resize(swapchain_image_views_.size());
  for (size_t i = 0; i < swapchain_image_views_.size(); ++i) {
    VkImageView attachments[] = {
      swapchain_image_views_[i]
    };

    VkFramebufferCreateInfo framebuffer_info { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
    framebuffer_info.renderPass      = render_pass_;
    framebuffer_info.attachmentCount = 1;
    framebuffer_info.pAttachments    = attachments;
    framebuffer_info.width           = swapchain_extent_.width;
    framebuffer_info.height          = swapchain_extent_.height;
    framebuffer_info.layers          = 1;

    VK_CHECK(vkCreateFramebuffer(device_, &framebuffer_info, allocator_, &swapchain_framebuffers_[i]));
  }
}

void VulkanRenderer::DestroySwapchainFramebuffers() {
  for (auto& framebuffer : swapchain_framebuffers_) {
    vkDestroyFramebuffer(device_, framebuffer, allocator_);
  }
}

}  // namespace Skeleton::Vulkan
