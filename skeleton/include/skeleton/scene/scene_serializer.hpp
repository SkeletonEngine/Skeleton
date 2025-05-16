// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <string>
#include <entt/entt.hpp>

namespace Skeleton {

entt::registry LoadScene(const std::string& path);
void SaveScene(const std::string& path, entt::registry* scene);

}  // namespace Skeleton
