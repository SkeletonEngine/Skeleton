// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/common/shader_reflection/shader_buffer_layout.hpp"
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
        case 3: return ShaderDataType::kFloat3;
        case 4: return ShaderDataType::kFloat4;
      }
    }
  }
}

ShaderBufferLayout::ShaderBufferLayout(const std::vector<uint32_t>& spv) {
  spirv_cross::Compiler compiler(spv);
  spirv_cross::ShaderResources resources = compiler.get_shader_resources();

  // The buffer elements may be out of order as we iterate over them,
  // so we'll place them into a map until we know how many there are
  std::map<int, ShaderBufferElement> element_map;

  for (auto& resource : resources.stage_inputs) {
    ShaderBufferElement element;
    element.name = resource.name;
    element.type = DeduceShaderDataType(compiler.get_type(resource.type_id));
    element.size = ShaderDataTypeSize(element.type);
    uint32_t location = compiler.get_decoration(resource.id, spv::DecorationLocation);
    element_map.emplace(location, element);
  }

  // Now that we know how many elements we have and their locations, we can calculate the offset of each one
  size_t offset = 0;
  for (auto& e : element_map) {
    ShaderBufferElement element = e.second;
    element.location = e.first;
    element.offset = offset;
    offset += element.size;
    elements_.push_back(element);
  }

  stride_ = offset;
}

size_t ShaderBufferLayout::GetStride() const {
  return stride_;
}

}  // namespace Skeleton
