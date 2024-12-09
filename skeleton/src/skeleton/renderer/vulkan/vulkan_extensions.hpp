#pragma once
#include "skeleton/core/core.hpp"

#include <vector>

namespace Skeleton::Vulkan {

#ifdef SK_BUILD_DEBUG
const std::vector<const char*> FindRequiredValidationLayers();
#endif
const std::vector<const char*> FindRequiredInstanceExtensions();
const std::vector<const char*> FindRequiredDeviceExtensions();

}
