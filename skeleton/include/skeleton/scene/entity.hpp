// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <string>
#include <entt/entt.hpp>

namespace Skeleton {

entt::entity CreateEmptyEntity(entt::registry* scene, entt::entity parent, const std::string& name);

}  // namespace Skeleton
