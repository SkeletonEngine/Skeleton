// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <vector>
#include <entt/entt.hpp>

namespace Skeleton {

struct ChildrenComponent {
  std::vector<entt::entity> children;
};

}  // namespace Skeleton
