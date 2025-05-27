// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <glm/glm.hpp>

namespace Skeleton {

// Like ImGui::DragFloat3, but with buttons to reset each axis
// to a specified value. The buttons are labeled "X", "Y", and "Z".
// Returns true if any value was changed
bool DrawXyzControl(const char* label, glm::vec3& values, float reset_value = 0.0f);

}  // namespace Skeleton
