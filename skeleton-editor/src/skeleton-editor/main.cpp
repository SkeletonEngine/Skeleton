// Copyright 2024-2025 SkeletonEngine

#include "skeleton/skeleton.hpp"
#include "skeleton/window/glfw/glfw_window.hpp"
#include "skeleton-editor/renderer/opengl/opengl_editor_renderer.hpp"
#include "skeleton-editor/renderer/vulkan/vulkan_editor_renderer.hpp"

int main() {
  Skeleton::RendererBackend backend = Skeleton::RendererBackend::kVulkan;

  Skeleton::Window* window = new Skeleton::GlfwWindow(backend);

  Skeleton::Renderer* renderer;
  switch (backend) {
    case Skeleton::RendererBackend::kOpenGl: renderer = new Skeleton::OpenGl::OpenGlEditorRenderer(window); break;
    case Skeleton::RendererBackend::kVulkan: renderer = new Skeleton::Vulkan::VulkanEditorRenderer(window); break;
    default: break;
  }

  window->SetTitle(renderer->GetRendererString());

  Skeleton::StartApplication(window, renderer);

  delete renderer;
  delete window;
}
