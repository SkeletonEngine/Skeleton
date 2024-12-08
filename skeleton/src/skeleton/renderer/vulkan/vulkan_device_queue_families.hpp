#pragma once
#include "skeleton/core/core.hpp"

#include <optional>
#include <volk.h>

namespace Skeleton::Vulkan {

class DeviceQueueFamilies {
public:
  DeviceQueueFamilies(VkPhysicalDevice physical_device);

public:
  bool IsComplete() const;

private:
  std::optional<uint32_t> graphics_family;
};

}
