// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

#include <vector>

namespace Skeleton::Vulkan {

/* We only use validation layers in debug builds */
#ifdef SK_BUILD_DEBUG
const std::vector<const char*> FindRequiredValidationLayers();
#endif

const std::vector<const char*> FindRequiredInstanceExtensions();
const std::vector<const char*> FindRequiredDeviceExtensions();

}  // namespace Skeleton::Vulkan
