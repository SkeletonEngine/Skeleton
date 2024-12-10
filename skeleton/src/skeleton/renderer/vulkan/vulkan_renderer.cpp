// Copyright 2024 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"

namespace Skeleton::Vulkan {

VulkanRenderer::VulkanRenderer(const ApplicationSettings& settings, Window* window) : window(window) {
  CreateInstance();
  CreateWindowSurface();
  ChoosePhysicalDevice();
  CreateDevice();
}

VulkanRenderer::~VulkanRenderer() {
  DestroyDevice();
  DestroyWindowSurface();
  DestroyInstance();
}

}  // namespace Skeleton::Vulkan
