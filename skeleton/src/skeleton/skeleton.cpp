// Copyright 2024-2025 SkeletonEngine

#include "skeleton/skeleton.hpp"
#include "skeleton/core/core.hpp"

#include "skeleton/renderer/opengl/opengl_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/window/glfw/glfw_window.hpp"

namespace Skeleton {

void StartApplication(Window* window, Renderer* renderer) {
  while (window->IsOpen()) {
    window->PollEvents();
    renderer->RenderFrame();
  }
}

}  // namespace Skeleton
