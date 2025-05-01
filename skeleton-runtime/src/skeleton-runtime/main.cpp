// Copyright 2024-2025 SkeletonEngine

#include <entt/entt.hpp>
#include "skeleton/skeleton.hpp"
#include "skeleton/window/glfw/glfw_window.hpp"
#include "skeleton/renderer/opengl/opengl_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"

int main() {
  Skeleton::WindowSettings window_settings;
  window_settings.backend          = Skeleton::WindowBackend::kGlfw;
  window_settings.renderer_backend = Skeleton::RendererBackend::kVulkan;
  window_settings.open_maximized   = false;
  Skeleton::Window* window = new Skeleton::GlfwWindow(window_settings);

  Skeleton::RendererSettings renderer_settings;
  renderer_settings.window = window;
  renderer_settings.vsync  = true;
  Skeleton::Renderer* renderer;
  switch (window_settings.renderer_backend) {
    case Skeleton::RendererBackend::kOpenGl: renderer = new Skeleton::OpenGl::OpenGlRenderer(renderer_settings); break;
    case Skeleton::RendererBackend::kVulkan: renderer = new Skeleton::Vulkan::VulkanRenderer(renderer_settings); break;
    default: break;
  }

  window->SetTitle(renderer->GetRendererString());

  Skeleton::StartApplication(window, renderer);

  delete renderer;
  delete window;
}
