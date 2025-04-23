// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include "skeleton/renderer/renderer_backend.hpp"
#include "skeleton/window/window_backend.hpp"

namespace Skeleton {

struct WindowSettings {
  WindowBackend   backend          = WindowBackend::kGlfw;
  RendererBackend renderer_backend = RendererBackend::kVulkan;
  bool            open_maximized   = false;
};

}  // namespace Skeleton
