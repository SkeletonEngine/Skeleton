#include "skeleton/renderer/vulkan/vulkan_swapchain_support.hpp"
#include "skeleton/core/core.hpp"

namespace Skeleton::Vulkan {

SwapchainSupportDetails::SwapchainSupportDetails(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
  /* Fetch the surface capabilities */
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &capabilities);
  
  /* Fetch the list of supported surface formats */
  uint32_t format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);
  if (format_count != 0) {
    formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, formats.data());
  }
  
  /* Fetch the list of supported present modes */
  uint32_t present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);
  if (present_mode_count != 0) {
    present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes.data());
  }
}

bool SwapchainSupportDetails::IsAdequate() const {
  /* We'll consider a physical device and surface combo suitable for our purposes if it supports
     at least one surface format and at least one present mode */
  return !formats.empty() && !present_modes.empty();
}

}
