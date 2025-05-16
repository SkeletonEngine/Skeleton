// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#define UUID_SYSTEM_GENERATOR
#include <uuid.h>

namespace Skeleton {

struct UuidComponent {
  const uuids::uuid uuid;
};

}  // namespace Skeleton
