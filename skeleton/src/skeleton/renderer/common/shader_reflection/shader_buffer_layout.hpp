// Copyright 2024-2025 SkeletonEngine

// Wrapper around a list of ShaderBufferElement
// Iterable object representing the layout of a shader uniform buffer, vertex buffer, etc.

#pragma once
#include "skeleton/core/core.hpp"

#include <cstdint>
#include <vector>
#include "skeleton/renderer/common/shader_reflection/shader_buffer_element.hpp"

namespace Skeleton {

class ShaderBufferLayout {
 public:
  ShaderBufferLayout(const std::vector<uint32_t>& spv);

 public:
  size_t GetStride() const;

 public:
	inline std::vector<ShaderBufferElement>::iterator begin()             { return elements_.begin(); }
	inline std::vector<ShaderBufferElement>::iterator end()               { return elements_.end();   }
	inline std::vector<ShaderBufferElement>::const_iterator begin() const { return elements_.begin(); }
	inline std::vector<ShaderBufferElement>::const_iterator end() const   { return elements_.end();   }

 private:
  std::vector<ShaderBufferElement> elements_;
  size_t stride_;
};

}  // namespace Skeleton
