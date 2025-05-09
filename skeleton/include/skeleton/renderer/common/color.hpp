// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <cstdint>
#include <initializer_list>

namespace Skeleton {

struct Color {
  explicit Color(uint32_t color);
  Color(float r, float g, float b, float a);
  Color(const std::initializer_list<float>& color);

  uint32_t AsUint32() const;

  union {
    float rgba[4];

    struct {
      float r;
      float g;
      float b;
      float a;
    };
  };
};

}  // namespace Skeleton
