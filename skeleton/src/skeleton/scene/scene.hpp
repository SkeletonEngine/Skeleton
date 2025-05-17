// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <entt/entt.hpp>
#include <uuid.h>

namespace Skeleton {

entt::entity GetRootNode(entt::registry* scene);
entt::entity GetNodeByUuid(entt::registry* scene, const uuids::uuid& uuid);

}  // namespace Skeleton
