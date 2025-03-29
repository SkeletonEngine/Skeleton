// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/core/core.hpp"

#ifdef SK_BUILD_DEBUG

namespace Skeleton::Vulkan {

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

  fprintf(stderr, "Validation Layer: %s\n", pCallbackData->pMessage);

  return VK_FALSE;
}

void VulkanRenderer::CreateDebugMessenger() {
  VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info { };
  PopulateDebugMessengerCreateInfo(&debug_messenger_info);
  vkCreateDebugUtilsMessengerEXT(instance_, &debug_messenger_info, allocator_, &debug_messenger_);
}

void VulkanRenderer::DestroyDebugMessenger() {
  vkDestroyDebugUtilsMessengerEXT(instance_, debug_messenger_, allocator_);
}

void VulkanRenderer::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* to_populate) {
  std::memset(to_populate, 0, sizeof(VkDebugUtilsMessengerCreateInfoEXT));
  to_populate->sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  to_populate->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                               | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  to_populate->messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                               | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                               | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  to_populate->pfnUserCallback = DebugCallback;
}

}  // namespace Skeleton::Vulkan

#endif  // SK_BUILD_DEBUG
