// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <map>
#include "skeleton/renderer/common/shader_reflection/shader_buffer_layout.hpp"

namespace Skeleton {

struct ShaderReflectionDetails {
  ShaderReflectionDetails(const std::vector<uint32_t>& spv);

  ShaderBufferLayout                     vertex_input_layout;
  std::map<uint32_t, ShaderBufferLayout> uniform_buffers;
};

}  // namespace Skeleton
