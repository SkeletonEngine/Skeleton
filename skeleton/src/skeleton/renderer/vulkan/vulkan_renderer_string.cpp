// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"

#include <sstream>

namespace Skeleton::Vulkan {

std::string VulkanRenderer::GetRendererString() const {
  VkPhysicalDeviceProperties props;
  vkGetPhysicalDeviceProperties(physical_device_, &props);
  uint32_t major_version = VK_VERSION_MAJOR(props.apiVersion);
  uint32_t minor_version = VK_VERSION_MINOR(props.apiVersion);
  uint32_t patch_version = VK_VERSION_PATCH(props.apiVersion);

  std::stringstream ss;
  ss << "Skeleton " << SK_VERSION_STRING;
  ss << " [Renderer: Vulkan " << major_version << "." << minor_version << "." << patch_version << "]";
  ss << " [GPU: " << props.deviceName << "]";

  return ss.str();
}

}  // namespace Skeleton::Vulkan
