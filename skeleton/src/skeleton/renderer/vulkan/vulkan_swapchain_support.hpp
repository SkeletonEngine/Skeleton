// Copyright 2024 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <volk.h>
#include <vector>
#include "skeleton/window/window.hpp"

namespace Skeleton::Vulkan {

class SwapchainSupportDetails {
 public:
  SwapchainSupportDetails(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

 public:
  /* Returns true if the physical device and surface combo are suitable for our purposes */
  bool IsAdequate() const;

  /* Returns the best available surface format for our purposes, hopefully SRGB */
  VkSurfaceFormatKHR ChooseSurfaceFormat() const;

  /* Returns the best available present mode depending on whether vsync is requested */
  VkPresentModeKHR ChoosePresentMode(bool vsync) const;

  /* Returns the extent (resolution) of the swapchain images we'll be rendering to.
     This is normally equal to the size of the window framebuffer, but could be different */
  VkExtent2D ChooseExtent(Window* window) const;

  uint32_t GetMinImageCount() const;
  uint32_t GetMaxImageCount() const;

 private:
  VkSurfaceCapabilitiesKHR capabilities_;
  std::vector<VkSurfaceFormatKHR> formats_;
  std::vector<VkPresentModeKHR> present_modes_;
};

}  // namespace Skeleton::Vulkan
