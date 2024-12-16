// Copyright 2024 SkeletonEngine

#include "skeleton/renderer/vulkan/pipeline/vulkan_graphics_pipeline.hpp"
#include "skeleton/core/core.hpp"

#include "skeleton/renderer/common/spv_file.hpp"

namespace Skeleton::Vulkan {

GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineSettings& settings) {
  std::vector<char> vert_spv = ReadSpvFile(settings.vert_path);
  std::vector<char> frag_spv = ReadSpvFile(settings.frag_path);
}

GraphicsPipeline::~GraphicsPipeline() {
}

}  // namespace Skeleton::Vulkan
