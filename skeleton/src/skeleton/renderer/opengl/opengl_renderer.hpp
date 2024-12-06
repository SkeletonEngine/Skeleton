#pragma once
#include "skeleton/core/core.hpp"

#include "skeleton/application_settings.hpp"
#include "skeleton/renderer/renderer.hpp"

namespace Skeleton {

class OpenGlRenderer : public Renderer {
public:
  OpenGlRenderer(const ApplicationSettings& settings);
};

}

