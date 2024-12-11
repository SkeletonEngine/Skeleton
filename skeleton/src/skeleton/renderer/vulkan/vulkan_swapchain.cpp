// Copyright 2024 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <algorithm>
#include <GLFW/glfw3.h>
#include "skeleton/renderer/vulkan/vulkan_check.hpp"
#include "skeleton/renderer/vulkan/vulkan_device_queue_families.hpp"
#include "skeleton/renderer/vulkan/vulkan_swapchain_support.hpp"

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateSwapchain() {
  /* Query the physical device and surface and choose the surface format and present mode for the swapchain */
  SwapchainSupportDetails swapchain_support(physical_device_, surface_);
  VkSurfaceFormatKHR surface_format = swapchain_support.ChooseSurfaceFormat();
  VkPresentModeKHR present_mode = swapchain_support.ChoosePresentMode(vsync_);

  /* Choose the swap extent (resolution) of the swapchain images we'll be rendering to */
  VkExtent2D extent = swapchain_support.ChooseExtent(window_);

  /* We request one more image than the minimum to reduce the probability that we end up having to wait on the
     driver to complete operations to resume rendering. */
  uint32_t image_count = swapchain_support.capabilities.minImageCount + 1;
  /* Make sure we don't exceed the max image count supported by the GPU */
  if (swapchain_support.capabilities.maxImageCount > 0 && image_count > swapchain_support.capabilities.maxImageCount) {
    image_count = swapchain_support.capabilities.maxImageCount;
  }

  /* Query device queue family indices so that we can figure out the swapchain sharing mode */
  DeviceQueueFamilies indices(physical_device_, surface_);
  uint32_t queue_family_indices[] = { indices.GraphicsFamilyIndex(), indices.PresentFamilyIndex() };

  /* Create the swapchain */
  VkSwapchainCreateInfoKHR swapchain_info { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
  swapchain_info.surface          = surface_;
  swapchain_info.minImageCount    = image_count;
  swapchain_info.imageFormat      = surface_format.format;
  swapchain_info.imageColorSpace  = surface_format.colorSpace;
  swapchain_info.imageExtent      = extent;
  swapchain_info.imageArrayLayers = 1;
  swapchain_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapchain_info.preTransform     = swapchain_support.capabilities.currentTransform;
  swapchain_info.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapchain_info.presentMode      = present_mode;
  swapchain_info.clipped          = VK_TRUE;
  swapchain_info.oldSwapchain     = VK_NULL_HANDLE;

  /* To avoid having to manage ownership transitions, we use VK_SHARING_MODE_CONCURRENT 
     for GPUs with separate graphics and compure queues */
  if (indices.GraphicsFamilyIndex() != indices.PresentFamilyIndex()) {
    swapchain_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
    swapchain_info.queueFamilyIndexCount = 2;
    swapchain_info.pQueueFamilyIndices   = queue_family_indices;
  } else {
    swapchain_info.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
  }

  VK_CHECK(vkCreateSwapchainKHR(device_, &swapchain_info, allocator_, &swapchain_));
}

void VulkanRenderer::DestroySwapchain() {
  vkDestroySwapchainKHR(device_, swapchain_, allocator_);
}

}  // namespace Skeleton::Vulkan
