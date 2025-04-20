// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/opengl/opengl_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <string>

namespace Skeleton::OpenGl {

OpenGlRenderer::OpenGlRenderer(Window* window) {
}

void OpenGlRenderer::RenderFrame() {
}

std::string OpenGlRenderer::GetRendererString() const {
  return "OpenGl";
}

}  // namespace Skeleton::OpenGl
