// Copyright 2024-2025 SkeletonEngine

#pragma once
#include <volk.h>
#include "skeleton/core/core.hpp"

namespace Skeleton::Vulkan {

struct GraphicsPipelineSettings {
  const char* vert_path = nullptr;
  const char* frag_path = nullptr;

  VkAllocationCallbacks* allocator;
  VkDevice               device;
};

}  // namespace Skeleton::Vulkan
