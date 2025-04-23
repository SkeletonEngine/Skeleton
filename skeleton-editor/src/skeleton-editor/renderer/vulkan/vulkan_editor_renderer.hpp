// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"

namespace Skeleton::Vulkan {

class VulkanEditorRenderer : public VulkanRenderer {
 public:
  VulkanEditorRenderer(Window* window);
  virtual ~VulkanEditorRenderer();

 public:
  virtual void RenderFrame() override;

 private:
  void PerformImguiRenderPass();

 private:
  void CreateImguiDescriptorPool();
  void DestroyImguiDescriptorPool();
  void CreateImguiRenderPass();
  void DestroyImguiRenderPass();
  void CreateViewportFramebuffer(size_t i);
  void DestroyViewportFramebuffer(size_t i);

 private:
  VkDescriptorPool             imgui_descriptor_pool_;
  VkRenderPass                 imgui_render_pass_;
  std::vector<VkImage>         editor_viewport_images_;
  std::vector<VmaAllocation>   editor_viewport_image_allocations_;
  std::vector<VkImageView>     editor_viewport_image_views_;
  std::vector<VkFramebuffer>   editor_viewport_framebuffers_;
  std::vector<VkSampler>       editor_viewport_samplers_;
  std::vector<VkDescriptorSet> editor_viewport_descriptor_sets_;
  VkExtent2D                   editor_viewport_extent_ { 1, 1 };
  bool                         editor_viewport_minimized_ = false;
  std::vector<bool>            editor_viewport_framebuffers_dirty_;
};

}  // namespace Skeleton::OpenGl
