#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <map>
#include <set>
#include <string>
#include <vector>
#include <volk.h>
#include "skeleton/renderer/vulkan/vulkan_device_queue_families.hpp"

namespace Skeleton::Vulkan {

static bool DeviceSupportsRequiredExtensions(VkPhysicalDevice physical_device) {
  const std::vector<const char*> kDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };

  /* Grab a list of all the extensions the device supports */
  uint32_t extension_count;
  vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);
  std::vector<VkExtensionProperties> available_extensions(extension_count);
  vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, available_extensions.data());
  
  /* Check each device extension we require is in the list we retrieved */
  std::set<std::string> required_extensions(kDeviceExtensions.begin(), kDeviceExtensions.end());
  for (const auto& extension : available_extensions) {
    required_extensions.erase(extension.extensionName);
  }
  
  return required_extensions.empty();
}

static bool DeviceMeetsBasicStandards(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
  /* Ensure the device supports graphics and presentation to a surface */
  DeviceQueueFamilies queues(physical_device, surface);
  if (!queues.IsComplete()) {
    return false;
  }

  /* Ensure the device supports rendering to a swapchain */
  if (!DeviceSupportsRequiredExtensions(physical_device)) {
    return false;
  }

  return true;
}

static int RateDeviceSuitability(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
  if (!DeviceMeetsBasicStandards(physical_device, surface)) return 0;
  
  int score = 0;
  
  /* Prioritize discrete GPUs */
  VkPhysicalDeviceProperties props;
  vkGetPhysicalDeviceProperties(physical_device, &props);
  if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 1000;
  }

  /* Prioritize devices with higher max texture size */
  score += props.limits.maxImageDimension2D;
  
  return score;
}

void VulkanRenderer::ChoosePhysicalDevice() {
  /* Find the count of vulkan capable physical devices */
  uint32_t physical_device_count;
  vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);
  
  /* Check that we have at least one vulkan capable physical device */
  SK_ASSERT(physical_device_count);
  
  /* Get   information about the available devices */
  std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
  vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data());
  
  /* Rate all devices and sort candidates by score, descending */
  std::multimap<int, VkPhysicalDevice> candidates;
  for (const auto& device : physical_devices) {
    int score = RateDeviceSuitability(device, surface);
    candidates.insert({ score, device });
  }

  /* Check whether the best candidate is suitable at all */
  SK_ASSERT(candidates.rbegin()->first > 0);
  
  /* Use the best device */
  physical_device = candidates.rbegin()->second;
}

}
