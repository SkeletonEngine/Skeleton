#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <map>
#include <vector>
#include <volk.h>

namespace Skeleton {

static bool DeviceMeetsBasicStandards(VkPhysicalDevice physical_device) {
  return true;
}

static int RateDeviceSuitability(VkPhysicalDevice physical_device) {
  if (!DeviceMeetsBasicStandards(physical_device)) return 0;
  
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
  
  /* Rate all devices and sort candidates by descending score */
  std::multimap<int, VkPhysicalDevice> candidates;
  for (const auto& device : physical_devices) {
    int score = RateDeviceSuitability(device);
    candidates.insert({ score, device });
  }

  /* Check whether the best candidate is suitable at all */
  SK_ASSERT(candidates.rbegin()->first > 0);
  
  /* Use the best device */
  physical_device = candidates.rbegin()->second;
}

}
