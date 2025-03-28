// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

namespace Skeleton {

class Renderer {
 public:
  virtual ~Renderer() { }
 
 public:
  virtual void RenderFrame() = 0;
};

}  // namespace Skeleton
