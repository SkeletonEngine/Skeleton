// Copyright 2024 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"

namespace Skeleton::Vulkan {

VulkanRenderer::VulkanRenderer(const ApplicationSettings& settings, Window* window)
    : window_(window), vsync_(settings.renderer.vsync) {
  CreateInstance();
  CreateWindowSurface();
  ChoosePhysicalDevice();
  CreateDevice();
  CreateSwapchain();
  CreateSwapchainImageViews();

  GraphicsPipelineSettings pipeline_settings;
  pipeline_settings.vert_path = "build/shaders/test.vert.spv";
  pipeline_settings.frag_path = "build/shaders/test.frag.spv";
  pipeline_ = new GraphicsPipeline(pipeline_settings);
}

VulkanRenderer::~VulkanRenderer() {
  delete pipeline_;

  DestroySwapchainImageViews();
  DestroySwapchain();
  DestroyDevice();
  DestroyWindowSurface();
  DestroyInstance();
}

}  // namespace Skeleton::Vulkan
