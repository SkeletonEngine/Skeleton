// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/pipeline/vulkan_graphics_pipeline.hpp"
#include "skeleton/core/core.hpp"

#include <volk.h>
#include "skeleton/renderer/common/spv_file.hpp"
#include "skeleton/renderer/vulkan/vulkan_check.hpp"

namespace Skeleton::Vulkan {

GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineSettings& settings) {
  std::vector<char> vert_spv = ReadSpvFile(settings.vert_path);
  std::vector<char> frag_spv = ReadSpvFile(settings.frag_path);

  // VkShaderModule vert_module = CreateShaderModule(vert_spv);
  // VkShaderModule frag_module = CreateShaderModule(frag_spv);
}

GraphicsPipeline::~GraphicsPipeline() {
}

}  // namespace Skeleton::Vulkan
