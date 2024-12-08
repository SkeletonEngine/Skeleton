#include "skeleton/skeleton.hpp"
#include "skeleton/core/core.hpp"

#include "skeleton/renderer/opengl/opengl_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/window/window.hpp"

namespace Skeleton {

void StartApplication(const ApplicationSettings& settings) {
  Window window(settings);
  
  Renderer* renderer;
  switch (settings.renderer.backend) {
    case RendererBackend::kOpenGl: renderer = new OpenGl::OpenGlRenderer(settings, &window); break;
    case RendererBackend::kVulkan: renderer = new Vulkan::VulkanRenderer(settings, &window); break;
    default:
      SK_DEBUGBREAK();
      return;
  }
  
  while (window.IsOpen()) {
    window.PollEvents();
  }
  
  delete renderer;
}

}
