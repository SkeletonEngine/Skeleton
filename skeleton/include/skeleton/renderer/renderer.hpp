// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <string>
#include "skeleton/renderer/renderer_backend.hpp"

namespace Skeleton {

class Renderer {
 public:
  virtual ~Renderer() { }

 public:
  virtual RendererBackend GetBackend() const = 0;

 public:
  virtual void RenderFrame() = 0;

 public:
  virtual std::string GetRendererString() const = 0;
};

}  // namespace Skeleton
