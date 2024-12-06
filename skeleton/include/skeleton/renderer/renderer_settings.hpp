#pragma once
#include "skeleton/core/core.hpp"

#include "skeleton/renderer/renderer_backend.hpp"

namespace Skeleton {

struct RendererSettings {
  RendererBackend backend = RendererBackend::kVulkan;
};

}
