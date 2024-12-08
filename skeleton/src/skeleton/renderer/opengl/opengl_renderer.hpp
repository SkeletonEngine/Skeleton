#pragma once
#include "skeleton/core/core.hpp"

#include "skeleton/application_settings.hpp"
#include "skeleton/renderer/renderer.hpp"
#include "skeleton/window/window.hpp"

namespace Skeleton::OpenGl {

class OpenGlRenderer : public Renderer {
public:
  OpenGlRenderer(const ApplicationSettings& settings, Window* window);
};

}

