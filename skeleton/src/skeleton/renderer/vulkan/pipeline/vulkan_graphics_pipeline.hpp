// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <vector>
#include "skeleton/renderer/vulkan/pipeline/vulkan_graphics_pipeline_settings.hpp"

namespace Skeleton::Vulkan {

class GraphicsPipeline {
public:
  GraphicsPipeline(const GraphicsPipelineSettings& settings);
  ~GraphicsPipeline();

private:
  VkShaderModule CreateShaderModule(const std::vector<char>& spv);

private:
  VkAllocationCallbacks* allocator_;
  VkDevice               device_;

  VkPipelineLayout layout_;
};

}  // namespace Skeleton::Vulkan
