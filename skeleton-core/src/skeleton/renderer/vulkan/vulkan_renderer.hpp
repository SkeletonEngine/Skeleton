#pragma once

#include "skeleton/application_settings.hpp"
#include "skeleton/renderer/renderer.hpp"

namespace Skeleton {

class VulkanRenderer : public Renderer {
public:
  VulkanRenderer(const ApplicationSettings& settings);
};

}
