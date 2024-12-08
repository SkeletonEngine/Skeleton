#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <volk.h>
#include "skeleton/renderer/vulkan/vulkan_check.hpp"
#include "skeleton/renderer/vulkan/vulkan_device_queue_families.hpp"

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateDevice() {
  /* Query the physical device to find the indices of the graphics and present queues */
  DeviceQueueFamilies queue_families(physical_device, surface);

  /* Priority of all queues will be 1 */
  float priority = 1.0f;
  
  /* Create the graphics queue */
  VkDeviceQueueCreateInfo queue_info { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
  queue_info.queueFamilyIndex = queue_families.GraphicsFamilyIndex();
  queue_info.queueCount = 1;
  queue_info.pQueuePriorities = &priority;

  /* We don't need any special device features for now */
  VkPhysicalDeviceFeatures device_features { };

  /* Create the VkDevice */
  VkDeviceCreateInfo device_info { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
  device_info.pQueueCreateInfos = &queue_info;
  device_info.queueCreateInfoCount = 1;
  device_info.pEnabledFeatures = &device_features;
  VK_CHECK(vkCreateDevice(physical_device, &device_info, allocator, &device));

  /* Retrieve graphics queue handle */
  vkGetDeviceQueue(device, queue_families.GraphicsFamilyIndex(), 0, &graphics_queue);
}

void VulkanRenderer::DestroyDevice() {
  vkDestroyDevice(device, allocator);
}

}
