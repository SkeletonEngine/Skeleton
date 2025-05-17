// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <glm/glm.hpp>

namespace Skeleton {

struct CameraComponent {
  float fov;
  float clip_near;
  float clip_far;

  // Should not be visible in the editor
  bool projection_matrix_dirty = false;
};

}  // namespace Skeleton
