// Copyright 2024-2025 SkeletonEngine

// Wrapper around a list of ShaderBufferElement
// Iterable object representing the layout of a shader uniform buffer, vertex buffer, etc.

#pragma once
#include "skeleton/core/core.hpp"

#include <cstdint>
#include <map>
#include <vector>
#include "skeleton/renderer/common/shader_reflection/shader_buffer_element.hpp"

namespace Skeleton {

class ShaderBufferLayout {
 public:
  ShaderBufferLayout() = default;
  explicit ShaderBufferLayout(const std::vector<ShaderBufferElement>& elements, std::optional<uint32_t> binding);
  explicit ShaderBufferLayout(const std::map<uint32_t, ShaderBufferElement>& element_map, std::optional<uint32_t> binding);

 public:
  uint32_t GetSize() const;
  std::optional<uint32_t> GetBinding() const;

 public:
  inline std::vector<ShaderBufferElement>::iterator begin()             { return elements_.begin(); }
  inline std::vector<ShaderBufferElement>::iterator end()               { return elements_.end();   }
  inline std::vector<ShaderBufferElement>::const_iterator begin() const { return elements_.begin(); }
  inline std::vector<ShaderBufferElement>::const_iterator end() const   { return elements_.end();   }

 private:
  std::vector<ShaderBufferElement> elements_;
  uint32_t size_ = 0;
  std::optional<uint32_t> binding_;
};

}  // namespace Skeleton
