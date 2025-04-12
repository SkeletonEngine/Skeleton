// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/common/shader_reflection/shader_reflection_details.hpp"
#include "skeleton/core/core.hpp"

#include <map>
#include <vector>
#include <spirv_cross.hpp>

namespace Skeleton {

static ShaderDataType DeduceShaderDataType(const spirv_cross::SPIRType& type) {
  switch (type.basetype) {
    case spirv_cross::SPIRType::BaseType::Float: {
      switch (type.vecsize) {
        case 1: return ShaderDataType::kFloat;
        case 2: return ShaderDataType::kFloat2;
        case 3: switch (type.columns) {
          case 1: return ShaderDataType::kFloat3;
          case 3: return ShaderDataType::kMat3;
        }
        case 4: switch (type.columns) {
          case 1: return ShaderDataType::kFloat4;
          case 4: return ShaderDataType::kMat4;
        }
      }
    }
  }
  return ShaderDataType::kFloat;
}

static ShaderBufferLayout ReflectVertexInputLayout(const spirv_cross::Compiler& compiler,
                                                   const spirv_cross::ShaderResources& resources) {
  // The buffer elements may be out of order as we iterate over them, so we'll place them into a map
  std::map<uint32_t, ShaderBufferElement> element_map;

  for (auto& vertex_input : resources.stage_inputs) {
    ShaderBufferElement element;
    element.name = vertex_input.name;
    element.type = DeduceShaderDataType(compiler.get_type(vertex_input.type_id));
    element.size = ShaderDataTypeSize(element.type);
    uint32_t location = compiler.get_decoration(vertex_input.id, spv::DecorationLocation);
    element_map.emplace(location, element);
  }

  return ShaderBufferLayout { element_map, std::nullopt };
}

static ShaderBufferLayout ReflectUniformBufferLayout(
    const spirv_cross::Compiler& compiler, const spirv_cross::ShaderResources& resources,
    const spirv_cross::Resource& uniform_buffer) {
  // Iterate over the members of the uniform buffer and create a ShaderBufferLayout
  std::vector<ShaderBufferElement> elements;
  const auto& buffer_type = compiler.get_type(uniform_buffer.type_id);

  // For each element in the uniform buffer, create a ShaderBufferElement
  for (uint32_t i = 0; i < buffer_type.member_types.size(); ++i) {
    ShaderBufferElement element;
    element.name = compiler.get_member_name(buffer_type.self, i);
    uint32_t member_id = buffer_type.member_types[i];
    element.type = DeduceShaderDataType(compiler.get_type(member_id));
    element.size = ShaderDataTypeSize(element.type);
    elements.push_back(element);
  }

  uint32_t binding = compiler.get_decoration(uniform_buffer.id, spv::DecorationLocation);
  return ShaderBufferLayout { elements, binding };
}

ShaderReflectionDetails::ShaderReflectionDetails(const std::vector<uint32_t>& spv) {
  spirv_cross::Compiler compiler(spv);
  spirv_cross::ShaderResources resources = compiler.get_shader_resources();

  vertex_input_layout = ReflectVertexInputLayout(compiler, resources);

  for (auto& uniform_buffer : resources.uniform_buffers) {
    uniform_buffers.emplace(uniform_buffer.name, ReflectUniformBufferLayout(compiler, resources, uniform_buffer));
  }
}

}  // namespace Skeleton
