// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/core/core.hpp"

#include "skeleton/renderer/vulkan/vulkan_check.hpp"

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateRenderCommandBuffer() {
  VkCommandBufferAllocateInfo alloc_info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
  alloc_info.commandPool        = command_pool_;
  alloc_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandBufferCount = kMaxFramesInFlight;

  render_command_buffers_.resize(kMaxFramesInFlight);
  vkAllocateCommandBuffers(device_, &alloc_info, render_command_buffers_.data());
}

void VulkanRenderer::DestroyRenderCommandBuffer() {
  vkFreeCommandBuffers(device_, command_pool_, kMaxFramesInFlight, render_command_buffers_.data());
}

void VulkanRenderer::RecordRenderCommandBuffer(VkCommandBuffer command_buffer, uint32_t image_index) {
  /* Begin recording the command buffer with no flags or inheritance */
  VkCommandBufferBeginInfo begin_info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
  VK_CHECK(vkBeginCommandBuffer(command_buffer, &begin_info));

  /* Begin the render pass */
  VkRenderPassBeginInfo render_pass_info { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
  render_pass_info.renderPass  = render_pass_;
  render_pass_info.framebuffer = swapchain_framebuffers_[image_index];
  render_pass_info.renderArea.offset = { 0, 0 };
  render_pass_info.renderArea.extent = swapchain_extent_;
  VkClearValue clear_color = {{{ 0.2f, 0.4f, 0.6f, 1.0f }}};
  render_pass_info.clearValueCount = 1;
  render_pass_info.pClearValues = &clear_color;
  vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

  /* Bind the pipeline */
  vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline_);

  /* Set dynamic viewport and scissor */
  VkViewport viewport { };
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapchain_extent_.width);
  viewport.height = static_cast<float>(swapchain_extent_.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(command_buffer, 0, 1, &viewport);

  VkRect2D scissor { };
  scissor.offset = { 0, 0 };
  scissor.extent = swapchain_extent_;
  vkCmdSetScissor(command_buffer, 0, 1, &scissor);

  /* Issue draw command */
  vkCmdDraw(command_buffer, 3, 1, 0, 0);

  /* End the render pass */
  vkCmdEndRenderPass(command_buffer);

  /* End the command buffer */
  VK_CHECK(vkEndCommandBuffer(command_buffer));
}

}  // namespace Skeleton::Vulkan
