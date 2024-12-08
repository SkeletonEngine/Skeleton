#include "skeleton/renderer/vulkan/vulkan_device_queue_families.hpp"
#include "skeleton/core/core.hpp"

#include <vector>
#include <volk.h>

namespace Skeleton::Vulkan {

DeviceQueueFamilies::DeviceQueueFamilies(VkPhysicalDevice physical_device) {
  uint32_t queue_family_count;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

  for (uint32_t i = 0; i < queue_family_count; ++i) {
    const auto& queue_family = queue_families[i];

    if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      graphics_family = i;
    }

    if (IsComplete()) {
      break;
    }
  }
}

uint32_t DeviceQueueFamilies::GraphicsFamilyIndex() const {
  return graphics_family.value();
}

bool DeviceQueueFamilies::IsComplete() const {
  return graphics_family.has_value();
}

}
