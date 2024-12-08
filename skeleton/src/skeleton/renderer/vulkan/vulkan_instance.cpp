#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <vector>
#include <GLFW/glfw3.h>
#include "skeleton/renderer/vulkan/vulkan_check.hpp"

namespace Skeleton::Vulkan {

void VulkanRenderer::CreateInstance() {
  /* Load initial vulkan functions */
  VK_CHECK(volkInitialize());
  
  /* --- Application info --- */
  VkApplicationInfo app_info { VK_STRUCTURE_TYPE_APPLICATION_INFO };
  app_info.pEngineName   = "Skeleton";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

  /* For now we are just using Vulkan 1.0 features */
  app_info.apiVersion    = VK_API_VERSION_1_0;

  /* --- Instance extensions --- */
  /* Query GLFW for the list of extensions required to render to a surface on the current platform */
  uint32_t glfw_extension_count;
  const char** glfw_extensions;
  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
  std::vector<const char*> instance_extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
  int flags = 0;

  /* To use MoltenVk we must enable the portability extension and set the portability bit flag */
#ifdef SK_PLATFORM_MACOS
  instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

  /* In debug builds we enable the validation layers extension and specify which layers we'd like to use */
#ifdef SK_BUILD_DEBUG
  instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  const std::vector<const char*> kValidationLayers = {
    "VK_LAYER_KHRONOS_validation"
  };
#endif

  /* Create the VkInstance */
  VkInstanceCreateInfo instance_info { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
  instance_info.pApplicationInfo        = &app_info;
  instance_info.enabledExtensionCount   = (uint32_t)instance_extensions.size();
  instance_info.ppEnabledExtensionNames = instance_extensions.data();
  instance_info.flags                   = flags;
#ifdef SK_BUILD_DEBUG
  instance_info.enabledLayerCount       = (uint32_t)kValidationLayers.size();
  instance_info.ppEnabledLayerNames     = kValidationLayers.data();
#endif
  VK_CHECK(vkCreateInstance(&instance_info, allocator, &instance));
  
  /* Load vulkan instance functions */
  volkLoadInstance(instance);
}

void VulkanRenderer::DestroyInstance() {
  vkDestroyInstance(instance, allocator);
}

}
