// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

#include <optional>

namespace Skeleton::Vulkan {

class DeviceQueueFamilies {
 public:
  DeviceQueueFamilies(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

 public:
  uint32_t GraphicsFamilyIndex() const;
  uint32_t PresentFamilyIndex() const;
  bool IsComplete() const;

 private:
  std::optional<uint32_t> graphics_family_;
  std::optional<uint32_t> present_family_;
};

}  // namespace Skeleton::Vulkan
