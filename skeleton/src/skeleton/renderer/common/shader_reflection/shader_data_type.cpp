// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/common/shader_reflection/shader_data_type.hpp"
#include "skeleton/core/core.hpp"

namespace Skeleton {

uint32_t ShaderDataTypeSize(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::kFloat:  return sizeof(float);
    case ShaderDataType::kFloat2: return sizeof(float) * 2;
    case ShaderDataType::kFloat3: return sizeof(float) * 3;
    case ShaderDataType::kFloat4: return sizeof(float) * 4;
    case ShaderDataType::kMat3:   return sizeof(float) * 3 * 3;
    case ShaderDataType::kMat4:   return sizeof(float) * 4 * 4;
    default: return 0;
  }
}

}  // namespace Skeleton
