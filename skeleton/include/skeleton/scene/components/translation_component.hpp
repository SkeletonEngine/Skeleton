// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <glm/glm.hpp>

namespace Skeleton {

struct TranslationComponent {
  glm::vec3 translation;

  // Should not be visible in the editor
  bool translation_matrix_dirty = false;
};

}  // namespace Skeleton
