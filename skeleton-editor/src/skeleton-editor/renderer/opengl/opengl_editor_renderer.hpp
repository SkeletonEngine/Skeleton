// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include "skeleton/renderer/opengl/opengl_renderer.hpp"
#include "skeleton/renderer/renderer_settings.hpp"

namespace Skeleton::OpenGl {

class OpenGlEditorRenderer : public OpenGlRenderer {
 public:
  explicit OpenGlEditorRenderer(const RendererSettings& settings);
};

}  // namespace Skeleton::OpenGl
