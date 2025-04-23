// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

#include <GLFW/glfw3.h>
#include "skeleton/window/glfw/glfw_window.hpp"

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateWindowSurface() {
  // If we're using GLFW, we can use glfwCreateWindowSurface to call the appropriate surface
  // creation function for the current platform, e.g. vkCreateWin32SurfaceKHR
  if (window_->GetBackend() == WindowBackend::kGlfw) {
    GlfwWindow* glfw_window = dynamic_cast<GlfwWindow*>(window_);
    VK_CHECK(glfwCreateWindowSurface(instance_, glfw_window->GetNativeWindowHandle(), allocator_, &surface_));
  } else {
    // When we support other window systems, we'll need to add code to create surfaces with them here
    SK_DEBUGBREAK();
  }
}

void VulkanRenderer::DestroyWindowSurface() {
  vkDestroySurfaceKHR(instance_, surface_, allocator_);
}

}  // namespace Skeleton::Vulkan
