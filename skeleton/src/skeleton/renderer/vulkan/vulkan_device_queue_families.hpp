// Copyright 2024 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <optional>
#include <volk.h>

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
