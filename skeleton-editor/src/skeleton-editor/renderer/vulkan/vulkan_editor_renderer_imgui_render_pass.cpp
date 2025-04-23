// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/renderer/vulkan/vulkan_editor_renderer.hpp"
#include "skeleton/core/core.hpp"

namespace Skeleton::Vulkan {

void VulkanEditorRenderer::CreateImguiRenderPass() {
  // Imgui render pass
  // This render pass has no depth attachment and renders directly to the swapchain
  // If we're using Imgui, other render passes will render to framebuffers which we
  // will then use as textures within Imgui

  // We're rendering to a single framebuffer with just one color attachment
  VkAttachmentDescription color_attachment { };
  color_attachment.format         = swapchain_image_format_;
  color_attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
  color_attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color_attachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
  color_attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  color_attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  color_attachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_attachment_ref { };
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  // Indices into the pColorAttachments array correspond to layout directives in the fragment shader, e.g.
  // layout (location = 0) out vec4 o_color;
  VkSubpassDescription subpass { };
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_attachment_ref;

  // Create subpass dependency to take care of image layout transition
  VkSubpassDependency dependency { };
  dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass    = 0;
  dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  // Create the render pass
  VkRenderPassCreateInfo render_pass_info { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
  render_pass_info.attachmentCount = 1;
  render_pass_info.pAttachments    = &color_attachment;
  render_pass_info.subpassCount    = 1;
  render_pass_info.pSubpasses      = &subpass;
  render_pass_info.dependencyCount = 1;
  render_pass_info.pDependencies   = &dependency;

  VK_CHECK(vkCreateRenderPass(device_, &render_pass_info, allocator_, &imgui_render_pass_));
}

void VulkanEditorRenderer::DestroyImguiRenderPass() {
  vkDestroyRenderPass(device_, imgui_render_pass_, allocator_);
}

}  // namespace Skeleton::Vulkan
