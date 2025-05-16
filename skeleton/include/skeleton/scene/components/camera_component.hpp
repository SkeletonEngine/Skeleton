// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <glm/glm.hpp>

namespace Skeleton {

struct CameraComponent {
  float fov;
  float aspect_ratio;
  bool  fixed_aspect_ratio;
  float clip_near;
  float clip_far;
};

}  // namespace Skeleton
