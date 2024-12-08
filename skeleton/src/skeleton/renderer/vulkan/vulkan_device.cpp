#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <set>
#include <vector>
#include <volk.h>
#include "skeleton/renderer/vulkan/vulkan_check.hpp"
#include "skeleton/renderer/vulkan/vulkan_device_queue_families.hpp"

namespace Skeleton::Vulkan {

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
  device_info.pQueueCreateInfos    = queue_infos.data();
  device_info.queueCreateInfoCount = (uint32_t)queue_infos.size();
  device_info.pEnabledFeatures     = &device_features;
  VK_CHECK(vkCreateDevice(physical_device, &device_info, allocator, &device));

  /* Retrieve graphics and present queue handles */
  vkGetDeviceQueue(device, queue_families.GraphicsFamilyIndex(), 0, &graphics_queue);
  vkGetDeviceQueue(device, queue_families.PresentFamilyIndex(),  0, &present_queue);
}

void VulkanRenderer::DestroyDevice() {
  vkDestroyDevice(device, allocator);
}

}
