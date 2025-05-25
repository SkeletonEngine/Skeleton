// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <glm/glm.hpp>

namespace Skeleton {

void DrawXyzControl(const char* label, glm::vec3& values, float resetValue = 0.0f);

}  // namespace Skeleton
