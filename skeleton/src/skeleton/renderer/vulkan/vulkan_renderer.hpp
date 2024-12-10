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
  VkAllocationCallbacks* allocator_       = VK_NULL_HANDLE;
  VkInstance             instance_        = VK_NULL_HANDLE;
  VkPhysicalDevice       physical_device_ = VK_NULL_HANDLE;
  VkDevice               device_          = VK_NULL_HANDLE;
  VkQueue                graphics_queue_  = VK_NULL_HANDLE;
  VkQueue                present_queue_   = VK_NULL_HANDLE;
  VkSurfaceKHR           surface_         = VK_NULL_HANDLE;
};

}
