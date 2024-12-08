#pragma once

#include <volk.h>
#include "skeleton/application_settings.hpp"
#include "skeleton/renderer/renderer.hpp"
#include "skeleton/window/window.hpp"

namespace Skeleton::Vulkan {

class VulkanRenderer : public Renderer {
public:
  VulkanRenderer(const ApplicationSettings& settings, Window* window);
  virtual ~VulkanRenderer();
  
private:
  void CreateInstance();
  void DestroyInstance();
  void CreateWindowSurface();
  void DestroyWindowSurface();
  void ChoosePhysicalDevice();
  void CreateDevice();
  void DestroyDevice();
  
private:
  /* Non-owning pointer to the window */
  Window* window;

private:
  /* Objects owned by the renderer */
  VkAllocationCallbacks* allocator       = VK_NULL_HANDLE;
  VkInstance             instance        = VK_NULL_HANDLE;
  VkPhysicalDevice       physical_device = VK_NULL_HANDLE;
  VkDevice               device          = VK_NULL_HANDLE;
  VkQueue                graphics_queue  = VK_NULL_HANDLE;
  VkQueue                present_queue   = VK_NULL_HANDLE;
  VkSurfaceKHR           surface         = VK_NULL_HANDLE;
};

}
