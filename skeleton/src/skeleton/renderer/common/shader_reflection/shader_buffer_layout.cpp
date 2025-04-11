// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/common/shader_reflection/shader_buffer_layout.hpp"
#include "skeleton/core/core.hpp"

#include <map>
#include <vector>

namespace Skeleton {

ShaderBufferLayout::ShaderBufferLayout(const std::vector<ShaderBufferElement>& elements) {
  size_t offset = 0;

  for (auto& e : elements) {
    ShaderBufferElement element = e;
    element.offset = offset;
    offset += element.size;
    elements_.push_back(element);
  }

  size_ = offset;
}

ShaderBufferLayout::ShaderBufferLayout(const std::map<uint32_t, ShaderBufferElement>& element_map) {
  size_t offset = 0;

  for (auto& e : element_map) {
    ShaderBufferElement element = e.second;
    element.location = e.first;
    element.offset = offset;
    offset += element.size;
    elements_.push_back(element);
  }

  size_ = offset;
}

size_t ShaderBufferLayout::GetSize() const {
  return size_;
}

}  // namespace Skeleton
