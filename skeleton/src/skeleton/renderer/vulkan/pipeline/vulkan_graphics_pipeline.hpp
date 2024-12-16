// Copyright 2024 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include "skeleton/renderer/vulkan/pipeline/vulkan_graphics_pipeline_settings.hpp"

namespace Skeleton::Vulkan {

class GraphicsPipeline {
public:
  GraphicsPipeline(const GraphicsPipelineSettings& settings);
  ~GraphicsPipeline();
};

}  // namespace Skeleton::Vulkan
