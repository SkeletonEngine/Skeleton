// Copyright 2024-2025 SkeletonEngine

// enumeration of shader data types used for reflection

#pragma once
#include "skeleton/core/core.hpp"

namespace Skeleton {

enum class ShaderDataType {
  kFloat, kFloat2, kFloat3, kFloat4,
  kMat3,  kMat4,
};

size_t ShaderDataTypeSize(ShaderDataType type);

}  // namespace Skeleton
