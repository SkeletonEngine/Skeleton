// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <cstdint>
#include <vector>

namespace Skeleton {

class ShaderReflectionDetails {
 public:
  explicit ShaderReflectionDetails(const std::vector<uint32_t>& spv);
};

}  // namespace Skeleton
