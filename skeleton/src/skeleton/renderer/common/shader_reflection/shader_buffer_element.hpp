// Copyright 2024-2025 SkeletonEngine

// Wrapper around ShaderDataType with some additional metadata attached

#pragma once
#include "skeleton/core/core.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include "skeleton/renderer/common/shader_reflection/shader_data_type.hpp"

namespace Skeleton {

struct ShaderBufferElement {
  std::string name;
  ShaderDataType type;
  uint32_t offset;
  uint32_t size;
  std::optional<uint32_t> location;
  std::optional<uint32_t> binding;
};

}  // namespace Skeleton
