// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <glm/glm.hpp>

namespace Skeleton {

struct CameraComponent {
  float fov;
  float aspect_ratio;
  bool fixed_aspect_ratio;
  float clip_near;
  float clip_far;
  glm::vec3 translation;
  glm::vec3 rotation;
  glm::mat4 cached_transform;
};

}  // namespace Skeleton
