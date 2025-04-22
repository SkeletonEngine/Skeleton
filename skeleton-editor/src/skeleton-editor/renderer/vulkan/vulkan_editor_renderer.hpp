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

 private:
  VkDescriptorPool imgui_descriptor_pool_;
  VkRenderPass     imgui_render_pass_;
};

}  // namespace Skeleton::OpenGl
