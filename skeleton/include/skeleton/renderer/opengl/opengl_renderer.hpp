// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <string>

#include "skeleton/renderer/renderer.hpp"
#include "skeleton/window/window.hpp"

namespace Skeleton::OpenGl {

class OpenGlRenderer : public Renderer {
 public:
  OpenGlRenderer(Window* window);

 public:
  virtual RendererBackend GetBackend() const override { return RendererBackend::kOpenGl; }

 public:
  virtual void RenderFrame() override;

 public:
  virtual std::string GetRendererString() const override;
};

}  // namespace Skeleton::OpenGl
