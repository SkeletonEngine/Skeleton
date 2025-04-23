// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include "skeleton/renderer/renderer_backend.hpp"
#include "skeleton/window/window.hpp"

namespace Skeleton {

struct RendererSettings {
  Window*         window;
  bool            vsync   = true;
};

}  // namespace Skeleton
