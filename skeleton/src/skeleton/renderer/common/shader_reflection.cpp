// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/common/shader_reflection.hpp"
#include "skeleton/core/core.hpp"

#include <vector>
#include <spirv_glsl.hpp>

namespace Skeleton {

ShaderReflectionDetails::ShaderReflectionDetails(const std::vector<uint32_t>& spv) {
  spirv_cross::CompilerGLSL glsl(spv);
  spirv_cross::ShaderResources resources = glsl.get_shader_resources();
}

}  // namespace Skeleton
