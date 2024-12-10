// Copyright 2024 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_extensions.hpp"
#include "skeleton/core/core.hpp"

#include <vector>
#include <GLFW/glfw3.h>
#include <volk.h>

namespace Skeleton::Vulkan {

/* We only use validation layers in debug builds */
#ifdef SK_BUILD_DEBUG
const std::vector<const char*> FindRequiredValidationLayers() {
  return { "VK_LAYER_KHRONOS_validation" };
}
#endif

const std::vector<const char*> FindRequiredInstanceExtensions() {
  /* Query GLFW for the list of extensions required to render to a surface on the current platform */
  uint32_t glfw_extension_count;
  const char** glfw_extensions;
  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
  std::vector<const char*> instance_extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

  /* To use MoltenVk we must enable the portability extension */
  /* We also require VK_KHR_get_physical_device_properties2 as this is required by the 
     VK_KHR_portability_subset device extension */
#ifdef SK_PLATFORM_MACOS
  instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  instance_extensions.push_back("VK_KHR_get_physical_device_properties2");
#endif

  /* We require debug utils to use validation layers */
#ifdef SK_BUILD_DEBUG
  instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

  return instance_extensions;
}

const std::vector<const char*> FindRequiredDeviceExtensions() {
  return {
    /* We obviously require the ability to render to a swapchain on every platform */
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,

    /* VK_KHR_portability_subset is required to use MoltenVk */
#ifdef SK_PLATFORM_MACOS
    "VK_KHR_portability_subset",
#endif
  };
}

}  // namespace Skeleton::Vulkan
