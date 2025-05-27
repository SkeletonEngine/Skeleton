// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <glm/glm.hpp>

namespace Skeleton {

// TransformComponent is used to store the transformation matrix of an entity.
// If an entity has a TranslationComponent, RotationComponent, or ScaleComponent, it will also have a TransformComponent
struct TransformComponent {
  glm::mat4 transform;

  // Should not be visible in the editor
  bool matrix_dirty = false;
};

}  // namespace Skeleton
