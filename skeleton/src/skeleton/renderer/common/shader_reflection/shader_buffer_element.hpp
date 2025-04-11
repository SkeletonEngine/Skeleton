// Copyright 2024-2025 SkeletonEngine

// Wrapper around ShaderDataType with some additional metadata attached

#pragma once
#include "skeleton/core/core.hpp"

#include <optional>
#include <string>
#include "skeleton/renderer/common/shader_reflection/shader_data_type.hpp"

namespace Skeleton {

struct ShaderBufferElement {
  std::string name;
  ShaderDataType type;
  size_t offset;
  size_t size;
  std::optional<size_t> location;
};

}  // namespace Skeleton
