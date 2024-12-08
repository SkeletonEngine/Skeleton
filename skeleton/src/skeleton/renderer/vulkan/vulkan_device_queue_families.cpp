#include "skeleton/renderer/vulkan/vulkan_device_queue_families.hpp"
#include "skeleton/core/core.hpp"

#include <vector>
#include <volk.h>

namespace Skeleton::Vulkan {

DeviceQueueFamilies::DeviceQueueFamilies(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
  /* Query the physical device to find its queue families */
  uint32_t queue_family_count;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

  /* Using a single queue for both graphics and presentation usually results in better performance, so we'll try to find a single queue that supports both */
  /* If there isn't a single queue that can do both, we'll use different queues */
  /* If we don't have at least one graphics queue and at least one present queue, this device isn't suitable for our purposes */
  struct QueueCapabilities {
    VkBool32 graphics = false;
    VkBool32 present  = false;
  };
  std::vector<QueueCapabilities> capabilities(queue_family_count);

  for (uint32_t i = 0; i < queue_family_count; ++i) {
    /* Query the queue family's ability to render graphics */
    capabilities[i].graphics = queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT;

    /* Query the queue family's ability to present to the window surface */
    vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &capabilities[i].present);

    /* If the current queue supports both, we're done */
    if (capabilities[i].graphics && capabilities[i].present) {
      graphics_family = i;
      present_family = i;
      return;
    }
  }

  /* If we get to the end of the list without finding a single queue that supports both graphics and present, pick different queues for each */
  for (uint32_t i = 0; i < queue_family_count; ++i) {
    if (!graphics_family.has_value() && capabilities[i].graphics) {
      graphics_family = i;
    }

    if (!present_family.has_value() && capabilities[i].present) {
      present_family = i;
    }

    /* We can stop as soon as we've found a graphics queue and a present queue */
    if (IsComplete()) {
      return;
    }
  }
}

uint32_t DeviceQueueFamilies::GraphicsFamilyIndex() const {
  return graphics_family.value();
}

uint32_t DeviceQueueFamilies::PresentFamilyIndex() const {
  return present_family.value();
}

bool DeviceQueueFamilies::IsComplete() const {
  return graphics_family.has_value();
}

}
