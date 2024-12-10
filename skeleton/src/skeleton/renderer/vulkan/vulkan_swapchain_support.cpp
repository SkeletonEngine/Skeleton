// Copyright 2024 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_swapchain_support.hpp"
#include "skeleton/core/core.hpp"

#include <algorithm>
#include "skeleton/window/window.hpp"

namespace Skeleton::Vulkan {

SwapchainSupportDetails::SwapchainSupportDetails(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
  /* Fetch the surface capabilities */
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &capabilities_);

  /* Fetch the list of supported surface formats */
  uint32_t format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);
  if (format_count != 0) {
    formats_.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, formats_.data());
  }

  /* Fetch the list of supported present modes */
  uint32_t present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);
  if (present_mode_count != 0) {
    present_modes_.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes_.data());
  }
}

bool SwapchainSupportDetails::IsAdequate() const {
  /* We'll consider a physical device and surface combo suitable for our purposes if it supports
     at least one surface format and at least one present mode */
  return !formats_.empty() && !present_modes_.empty();
}

VkSurfaceFormatKHR SwapchainSupportDetails::ChooseSurfaceFormat() const {
  /* Iterate over the list looking for SRGB, since this results in more accurate percieved colors */
  for (const auto& format : formats_) {
    if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return format;
    }
  }

  /* If we don't have an available SRGB format, we'll just pick any supported format */
  return formats_[0];
}

VkPresentModeKHR SwapchainSupportDetails::ChoosePresentMode(bool vsync) const {
  /* If vsync has been requested, our first preference is VK_PRESENT_MODE_IMMEDIATE_KHR */
  if (vsync && std::count(present_modes_.begin(), present_modes_.end(), VK_PRESENT_MODE_IMMEDIATE_KHR)) {
    return VK_PRESENT_MODE_IMMEDIATE_KHR;
  }

  /* If vsync has been requested or VK_PRESENT_MODE_IMMEDIATE_KHR is'nt available,
     our next preference is VK_PRESENT_MODE_MAILBOX_KHR */
  if (std::count(present_modes_.begin(), present_modes_.end(), VK_PRESENT_MODE_MAILBOX_KHR)) {
    return VK_PRESENT_MODE_MAILBOX_KHR;
  }
  
  /* If we can't use VK_PRESENT_MODE_MAILBOX_KHR we'll fall back to VK_PRESENT_MODE_FIFO_KHR,
     which is guaranteed to be available */
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapchainSupportDetails::ChooseExtent(Window* window) const {
  if (capabilities_.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities_.currentExtent;
  } else { 
    VkExtent2D actual_extent = {
      static_cast<uint32_t>(window->GetFramebufferWidth()),
      static_cast<uint32_t>(window->GetFramebufferHeight()),
    };
    
    actual_extent.width = std::clamp(actual_extent.width,
      capabilities_.minImageExtent.width,  capabilities_.maxImageExtent.width);
    actual_extent.height = std::clamp(actual_extent.height,
      capabilities_.minImageExtent.height, capabilities_.maxImageExtent.height);
    
    return actual_extent;
  }
}

uint32_t SwapchainSupportDetails::GetMinImageCount() const {
  return capabilities_.minImageCount;
}

uint32_t SwapchainSupportDetails::GetMaxImageCount() const {
  return capabilities_.maxImageCount;
}

}  // namespace Skeleton::Vulkan
