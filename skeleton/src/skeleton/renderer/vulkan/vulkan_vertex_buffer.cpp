// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateGpuMemoryAllocator() {
  // Since we are using volk to load vulkan functions, we need to explicitly feed VMA
  // the addresses of all the functions it will use
  VmaVulkanFunctions vulkan_functions { };
  vulkan_functions.vkGetInstanceProcAddr               = vkGetInstanceProcAddr;
  vulkan_functions.vkGetDeviceProcAddr                 = vkGetDeviceProcAddr;
  vulkan_functions.vkGetPhysicalDeviceProperties       = vkGetPhysicalDeviceProperties;
  vulkan_functions.vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties;
  vulkan_functions.vkAllocateMemory                    = vkAllocateMemory;
  vulkan_functions.vkFreeMemory                        = vkFreeMemory;
  vulkan_functions.vkMapMemory                         = vkMapMemory;
  vulkan_functions.vkUnmapMemory                       = vkUnmapMemory;
  vulkan_functions.vkFlushMappedMemoryRanges           = vkFlushMappedMemoryRanges;
  vulkan_functions.vkInvalidateMappedMemoryRanges      = vkInvalidateMappedMemoryRanges;
  vulkan_functions.vkBindBufferMemory                  = vkBindBufferMemory;
  vulkan_functions.vkBindImageMemory                   = vkBindImageMemory;
  vulkan_functions.vkGetBufferMemoryRequirements       = vkGetBufferMemoryRequirements;
  vulkan_functions.vkGetImageMemoryRequirements        = vkGetImageMemoryRequirements;
  vulkan_functions.vkCreateBuffer                      = vkCreateBuffer;
  vulkan_functions.vkDestroyBuffer                     = vkDestroyBuffer;
  vulkan_functions.vkCreateImage                       = vkCreateImage;
  vulkan_functions.vkDestroyImage                      = vkDestroyImage;
  vulkan_functions.vkCmdCopyBuffer                     = vkCmdCopyBuffer;

  VmaAllocatorCreateInfo allocator_create_info = { };
  allocator_create_info.flags            = 0;
  allocator_create_info.vulkanApiVersion = SK_VK_API_VERSION;
  allocator_create_info.physicalDevice   = physical_device_;
  allocator_create_info.device           = device_;
  allocator_create_info.instance         = instance_;
  allocator_create_info.pVulkanFunctions = &vulkan_functions;

  vmaCreateAllocator(&allocator_create_info, &vma_allocator_);
}

void VulkanRenderer::DestroyGpuMemoryAllocator() {
  vmaDestroyAllocator(vma_allocator_);
}

}  // namespace Skeleton::Vulkan
