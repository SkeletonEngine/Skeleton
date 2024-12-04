#include "vulkan_renderer.hpp"

#include <vector>
#include <GLFW/glfw3.h>
#include "skeleton/platform.hpp"
#include "skeleton/renderer/vulkan/vulkan_check.hpp"

namespace Skeleton {

VulkanRenderer::VulkanRenderer(const ApplicationSettings& settings) {
  VK_CHECK(volkInitialize());
  
  VkApplicationInfo app_info { VK_STRUCTURE_TYPE_APPLICATION_INFO };
  app_info.pEngineName   = "Skeleton";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion    = VK_API_VERSION_1_0;
  
  uint32_t glfw_extension_count;
  const char** glfw_extensions;
  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
  std::vector<const char*> instance_extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
#ifdef SK_PLATFORM_MACOS
  instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif
  
  VkInstanceCreateInfo instance_info { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
  instance_info.pApplicationInfo        = &app_info;
  instance_info.enabledExtensionCount   = (uint32_t)instance_extensions.size();
  instance_info.ppEnabledExtensionNames = instance_extensions.data();
#ifdef SK_PLATFORM_MACOS
  instance_info.flags                   = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
  
  VK_CHECK(vkCreateInstance(&instance_info, &allocator, &instance));
  volkLoadInstance(instance);
}

VulkanRenderer::~VulkanRenderer() {
  vkDestroyInstance(instance, &allocator);
}

}
