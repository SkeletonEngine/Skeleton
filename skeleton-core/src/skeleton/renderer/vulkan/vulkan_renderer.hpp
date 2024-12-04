#pragma once

#include <volk.h>
#include "skeleton/application_settings.hpp"
#include "skeleton/renderer/renderer.hpp"

namespace Skeleton {

class VulkanRenderer : public Renderer {
public:
  VulkanRenderer(const ApplicationSettings& settings);
  ~VulkanRenderer();
  
private:
  VkAllocationCallbacks allocator;
  VkInstance            instance;
};

}
