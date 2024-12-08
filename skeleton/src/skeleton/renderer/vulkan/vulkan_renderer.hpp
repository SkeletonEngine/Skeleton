#pragma once

#include <volk.h>
#include "skeleton/application_settings.hpp"
#include "skeleton/renderer/renderer.hpp"

namespace Skeleton::Vulkan {

class VulkanRenderer : public Renderer {
public:
  VulkanRenderer(const ApplicationSettings& settings);
  virtual ~VulkanRenderer();
  
private:
  void CreateInstance();
  void DestroyInstance();
  void ChoosePhysicalDevice();
  void CreateDevice();
  void DestroyDevice();
  
private:
  VkAllocationCallbacks* allocator       = VK_NULL_HANDLE;
  VkInstance             instance        = VK_NULL_HANDLE;
  VkPhysicalDevice       physical_device = VK_NULL_HANDLE;
  VkDevice               device          = VK_NULL_HANDLE;
  VkQueue                graphics_queue  = VK_NULL_HANDLE;
};

}
