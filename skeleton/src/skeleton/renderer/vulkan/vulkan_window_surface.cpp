// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

#include <GLFW/glfw3.h>

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateWindowSurface() {
  /* glfwCreateWindowSurface will call the appropriate surface creation function for the current platform, 
     e.g. vkCreateWin32SurfaceKHR */
  VK_CHECK(glfwCreateWindowSurface(instance_, window_->GetGlfwWindowHandle(), allocator_, &surface_));
}

void VulkanRenderer::DestroyWindowSurface() {
  vkDestroySurfaceKHR(instance_, surface_, allocator_);
}

}  // namespace Skeleton::Vulkan
