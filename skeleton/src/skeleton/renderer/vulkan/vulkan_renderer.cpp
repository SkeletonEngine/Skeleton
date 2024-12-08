#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"

namespace Skeleton::Vulkan {

VulkanRenderer::VulkanRenderer(const ApplicationSettings& settings) {
  CreateInstance();
  ChoosePhysicalDevice();
  CreateDevice();
}

VulkanRenderer::~VulkanRenderer() {
  DestroyDevice();
  DestroyInstance();
}

}
