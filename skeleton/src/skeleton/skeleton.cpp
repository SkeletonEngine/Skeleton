#include "skeleton/skeleton.hpp"

#include "skeleton/renderer/opengl/opengl_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/window/window.hpp"

namespace Skeleton {

void StartApplication(const ApplicationSettings& settings) {
  Window window(settings);
  
  Renderer* renderer;
  switch (settings.renderer.backend) {
    case RendererBackend::kOpenGl: renderer = new OpenGlRenderer(settings); break;
    case RendererBackend::kVulkan: renderer = new VulkanRenderer(settings); break;
  }
  
  while (window.IsOpen()) {
    window.PollEvents();
  }
  
  delete renderer;
}

}