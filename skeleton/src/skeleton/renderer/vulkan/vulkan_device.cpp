#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <map>
#include <set>
#include <vector>
#include <volk.h>
#include "skeleton/renderer/vulkan/vulkan_check.hpp"
#include "skeleton/renderer/vulkan/vulkan_device_queue_families.hpp"

namespace Skeleton::Vulkan {

static const std::vector<const char*> kDeviceExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#ifdef SK_PLATFORM_MACOS
  "VK_KHR_portability_subset",
#endif
};

static bool DeviceSupportsRequiredExtensions(VkPhysicalDevice physical_device) {
  /* Grab a list of all the extensions the device supports */
  uint32_t extension_count;
  vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);
  std::vector<VkExtensionProperties> available_extensions(extension_count);
  vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, available_extensions.data());
  
  /* Check each device extension we require is in the list of extensions that the device supports */
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

void VulkanRenderer::CreateDevice() {
  /* Query the physical device to find the indices of the graphics and present queues */
  DeviceQueueFamilies queue_families(physical_device, surface);

  /* Priority of all queues will be 1 */
  float priority = 1.0f;
  
  /* We use a set so that if we are using the same queue for both graphics and present we will only create one queue */
  std::set<uint32_t> unique_queue_families = { queue_families.GraphicsFamilyIndex(), queue_families.PresentFamilyIndex() };
  std::vector<VkDeviceQueueCreateInfo> queue_infos(unique_queue_families.size());
  uint32_t i = 0;
  for (uint32_t queue_family : unique_queue_families) {
    queue_infos[i].sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_infos[i].queueFamilyIndex = queue_family;
    queue_infos[i].queueCount       = 1;
    queue_infos[i].pQueuePriorities = &priority;
    ++i;
  }

  /* We don't need any special device features for now */
  VkPhysicalDeviceFeatures device_features { };

  /* Create the VkDevice */
  VkDeviceCreateInfo device_info { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
  device_info.pQueueCreateInfos       = queue_infos.data();
  device_info.queueCreateInfoCount    = (uint32_t)queue_infos.size();
  device_info.pEnabledFeatures        = &device_features;
  device_info.ppEnabledExtensionNames = kDeviceExtensions.data();
  device_info.enabledExtensionCount   = (uint32_t)kDeviceExtensions.size();
  VK_CHECK(vkCreateDevice(physical_device, &device_info, allocator, &device));

  /* Retrieve graphics and present queue handles */
  vkGetDeviceQueue(device, queue_families.GraphicsFamilyIndex(), 0, &graphics_queue);
  vkGetDeviceQueue(device, queue_families.PresentFamilyIndex(),  0, &present_queue);
}

void VulkanRenderer::DestroyDevice() {
  vkDestroyDevice(device, allocator);
}

}
