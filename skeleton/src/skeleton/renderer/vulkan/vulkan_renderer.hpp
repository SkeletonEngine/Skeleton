#pragma once

#include <volk.h>
#include "skeleton/application_settings.hpp"
#include "skeleton/renderer/renderer.hpp"

namespace Skeleton {

class VulkanRenderer : public Renderer {
public:
  VulkanRenderer(const ApplicationSettings& settings);
  ~VulkanRenderer();
  
private:
  void CreateInstance();
  void ChoosePhysicalDevice();
  
private:
  VkAllocationCallbacks* allocator       = VK_NULL_HANDLE;
  VkInstance             instance        = VK_NULL_HANDLE;
  VkPhysicalDevice       physical_device = VK_NULL_HANDLE;
};

}
