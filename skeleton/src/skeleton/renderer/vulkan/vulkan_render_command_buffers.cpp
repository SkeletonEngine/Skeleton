// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

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

void VulkanRenderer::BeginRenderCommandBuffer() {
  // Reset the command buffer and delete all previously recorded commands
  vkResetCommandBuffer(render_command_buffers_[current_frame_], 0);

  // Begin recording the command buffer with no flags or inheritance
  VkCommandBufferBeginInfo begin_info { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
  VK_CHECK(vkBeginCommandBuffer(render_command_buffers_[current_frame_], &begin_info));
}

void VulkanRenderer::PerformSceneRenderPass() {
  // Begin the render pass
  VkRenderPassBeginInfo render_pass_info { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
  render_pass_info.renderPass  = render_pass_;
  render_pass_info.framebuffer = (*render_target_framebuffers_)[image_index_];
  render_pass_info.renderArea.offset = { 0, 0 };
  render_pass_info.renderArea.extent = *render_target_extent_;
  VkClearValue clear_color = {{{ 0.2f, 0.4f, 0.6f, 1.0f }}};
  render_pass_info.clearValueCount = 1;
  render_pass_info.pClearValues = &clear_color;
  vkCmdBeginRenderPass(render_command_buffers_[current_frame_], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

  // Bind the pipeline
  vkCmdBindPipeline(render_command_buffers_[current_frame_], VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline_);

  // Set dynamic viewport and scissor
  VkViewport viewport { };
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(render_target_extent_->width);
  viewport.height = static_cast<float>(render_target_extent_->height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(render_command_buffers_[current_frame_], 0, 1, &viewport);

  VkRect2D scissor { };
  scissor.offset = { 0, 0 };
  scissor.extent = *render_target_extent_;
  vkCmdSetScissor(render_command_buffers_[current_frame_], 0, 1, &scissor);

  // Bind the vertex buffer
  VkBuffer vertex_buffers[] = { vertex_buffer_ };
  VkDeviceSize offsets[] = { 0 };
  vkCmdBindVertexBuffers(render_command_buffers_[current_frame_], 0, 1, vertex_buffers, offsets);

  // Bind the index buffer
  vkCmdBindIndexBuffer(render_command_buffers_[current_frame_], index_buffer_, 0, VK_INDEX_TYPE_UINT16);

  // Bind descriptor sets
  for (const auto& uniform_buffer : uniform_buffers_) {
    vkCmdBindDescriptorSets(render_command_buffers_[current_frame_], VK_PIPELINE_BIND_POINT_GRAPHICS,
                            graphics_pipeline_layout_, 0, 1, &descriptor_sets_[current_frame_], 0, nullptr);
  }

  // Issue draw command
  vkCmdDrawIndexed(render_command_buffers_[current_frame_], index_count_, 1, 0, 0, 0);

  // End the render pass
  vkCmdEndRenderPass(render_command_buffers_[current_frame_]);
}

void VulkanRenderer::EndRenderCommandBuffer() {
  // End the command buffer
  VK_CHECK(vkEndCommandBuffer(render_command_buffers_[current_frame_]));
}

}  // namespace Skeleton::Vulkan
