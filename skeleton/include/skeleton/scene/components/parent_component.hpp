// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <entt/entt.hpp>

namespace Skeleton {

struct ParentComponent {
  const entt::entity parent;
};

}  // namespace Skeleton
