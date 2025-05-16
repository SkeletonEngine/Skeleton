// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

#include <vector>
#include <GLFW/glfw3.h>
#include "skeleton/core/version.hpp"
#include "skeleton/renderer/vulkan/vulkan_extensions.hpp"

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateInstance() {
  /* Load initial vulkan functions */
  VK_CHECK(volkInitialize());

  /* --- Application info --- */
  VkApplicationInfo app_info { VK_STRUCTURE_TYPE_APPLICATION_INFO };
  app_info.pEngineName   = "Skeleton";
  app_info.engineVersion = VK_MAKE_VERSION(SK_VERSION_MAJOR, SK_VERSION_MINOR, SK_VERSION_PATCH);
  app_info.apiVersion    = SK_VK_API_VERSION;

  /* Find the list of required instance extensions and the list of required validation layers */
#ifdef SK_BUILD_DEBUG
  const std::vector<const char*> kValidationLayers   = FindRequiredValidationLayers();
#endif
  const std::vector<const char*> kInstanceExtensions = FindRequiredInstanceExtensions();

  /* Create the VkInstance */
  VkInstanceCreateInfo instance_info { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
  instance_info.pApplicationInfo        = &app_info;
  instance_info.enabledExtensionCount   = static_cast<uint32_t>(kInstanceExtensions.size());
  instance_info.ppEnabledExtensionNames = kInstanceExtensions.data();

#ifdef SK_PLATFORM_MACOS
  instance_info.flags                   = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

#ifdef SK_BUILD_DEBUG
  instance_info.enabledLayerCount       = static_cast<uint32_t>(kValidationLayers.size());
  instance_info.ppEnabledLayerNames     = kValidationLayers.data();

  VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info;
  PopulateDebugMessengerCreateInfo(&debug_messenger_info);
  instance_info.pNext                   = &debug_messenger_info;
#endif

  VK_CHECK(vkCreateInstance(&instance_info, allocator_, &instance_));

  /* Load vulkan instance functions */
  volkLoadInstance(instance_);
}

void VulkanRenderer::DestroyInstance() {
  vkDestroyInstance(instance_, allocator_);
}

}  // namespace Skeleton::Vulkan
