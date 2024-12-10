#pragma once
#include "skeleton/core/core.hpp"

#include <volk.h>
#include <vector>

namespace Skeleton::Vulkan {

class SwapchainSupportDetails {
public:
  SwapchainSupportDetails(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
  
public:
  /* Returns true if the physical device and surface combo are suitable for our purposes */
  bool IsAdequate() const;
  
private:
  VkSurfaceCapabilitiesKHR capabilities_;
  std::vector<VkSurfaceFormatKHR> formats_;
  std::vector<VkPresentModeKHR> present_modes_;
};

}
