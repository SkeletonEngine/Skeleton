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
  /* Owned by the renderer */
  VkAllocationCallbacks* allocator_;
  VkDevice               device_;

private:
  /* Owned by the pipeline */
  VkPipelineLayout layout_;
  VkPipeline       pipeline_;
};

}  // namespace Skeleton::Vulkan
