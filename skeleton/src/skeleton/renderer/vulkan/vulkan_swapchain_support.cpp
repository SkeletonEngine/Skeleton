// Copyright 2024 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_swapchain_support.hpp"
#include "skeleton/core/core.hpp"

#include <algorithm>
#include "skeleton/window/window.hpp"

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

VkSurfaceFormatKHR SwapchainSupportDetails::ChooseSurfaceFormat() const {
  /* Iterate over the list looking for SRGB, since this results in more accurate percieved colors */
  for (const auto& format : formats) {
    if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return format;
    }
  }

  /* If we don't have an available SRGB format, we'll just pick any supported format */
  return formats[0];
}

VkPresentModeKHR SwapchainSupportDetails::ChoosePresentMode(bool vsync) const {
  /* If vsync has been requested, our first preference is VK_PRESENT_MODE_IMMEDIATE_KHR */
  if (vsync && std::count(present_modes.begin(), present_modes.end(), VK_PRESENT_MODE_IMMEDIATE_KHR)) {
    return VK_PRESENT_MODE_IMMEDIATE_KHR;
  }

  /* If vsync has been requested or VK_PRESENT_MODE_IMMEDIATE_KHR is'nt available,
     our next preference is VK_PRESENT_MODE_MAILBOX_KHR */
  if (std::count(present_modes.begin(), present_modes.end(), VK_PRESENT_MODE_MAILBOX_KHR)) {
    return VK_PRESENT_MODE_MAILBOX_KHR;
  }
  
  /* If we can't use VK_PRESENT_MODE_MAILBOX_KHR we'll fall back to VK_PRESENT_MODE_FIFO_KHR,
     which is guaranteed to be available */
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapchainSupportDetails::ChooseExtent(Window* window) const {
  /* Some window managers will indicate that the extent of the swapchain may not match 1:1 with the size of the
     window in pixels, e.g. when using a high DPI monitor, by setting currentExtent to UINT_MAX.
     Usually we'll just use the swapchain extent that Vulkan deduces from the surface. If we can't, we'll use GLFW
     to query the framebuffer size and use that instead */
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else { 
    VkExtent2D actual_extent = {
      static_cast<uint32_t>(window->GetFramebufferWidth()),
      static_cast<uint32_t>(window->GetFramebufferHeight()),
    };
    
    actual_extent.width = std::clamp(actual_extent.width,
      capabilities.minImageExtent.width,  capabilities.maxImageExtent.width);
    actual_extent.height = std::clamp(actual_extent.height,
      capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    
    return actual_extent;
  }
}

}  // namespace Skeleton::Vulkan
