// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/common/color.hpp"
#include "skeleton/core/core.hpp"

namespace Skeleton {

Color::Color(uint32_t color) {
  r = ((color >> 24) & 0xff) / 255.0f;
  g = ((color >> 16) & 0xff) / 255.0f;
  b = ((color >> 8)  & 0xff) / 255.0f;
  a = (color         & 0xff) / 255.0f;
}

Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {
}

Color::Color(const std::initializer_list<float>& color) {
  // A color should have either 3 or 4 channels
  SK_ASSERT(color.size() == 3 || color.size() == 4);

  if (color.size() == 4) {
    auto it = color.begin();
    r = *(it++);
    g = *(it++);
    b = *(it++);
    a = *(it);
  } else if (color.size() == 3) {
    auto it = color.begin();
    r = *(it++);
    g = *(it++);
    b = *(it);
    a = 1.0f;
  } else {
    r = g = b = a = 0.0f;
  }
}

uint32_t Color::AsUint32() const {
  return (static_cast<uint32_t>(r * 255) << 24) |
         (static_cast<uint32_t>(g * 255) << 16) |
         (static_cast<uint32_t>(b * 255) << 8)  |
         (static_cast<uint32_t>(a * 255));
}

}  // namespace Skeleton
