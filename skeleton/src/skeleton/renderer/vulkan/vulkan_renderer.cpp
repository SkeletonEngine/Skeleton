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
}

VulkanRenderer::~VulkanRenderer() {
  DestroySwapchain();
  DestroyDevice();
  DestroyWindowSurface();
  DestroyInstance();
}

}  // namespace Skeleton::Vulkan
