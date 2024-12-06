#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"

namespace Skeleton {

VulkanRenderer::VulkanRenderer(const ApplicationSettings& settings) {
  CreateInstance();
  ChoosePhysicalDevice();
}

VulkanRenderer::~VulkanRenderer() {
  vkDestroyInstance(instance, allocator);
}

}
