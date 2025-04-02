// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <vector>
#include <volk.h>
#include "skeleton/renderer/common/shader_reflection.hpp"
#include "skeleton/renderer/common/spv_file.hpp"
#include "skeleton/renderer/vulkan/vulkan_check.hpp"

namespace Skeleton::Vulkan {

static VkShaderModule CreateShaderModule(const std::vector<uint32_t>& spv, VkDevice device,
                                         VkAllocationCallbacks* allocator) {
  VkShaderModuleCreateInfo module_info { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
  module_info.codeSize = spv.size() * sizeof(uint32_t);
  module_info.pCode    = spv.data();

  VkShaderModule shader_module;
  VK_CHECK(vkCreateShaderModule(device, &module_info, allocator, &shader_module));
  return shader_module;
}

void VulkanRenderer::CreateGraphicsPipeline() {
  /* Hard code the shader paths for testing */
  const char* kVertPath = "build/shaders/test.vert.spv";
  const char* kFragPath = "build/shaders/test.frag.spv";

  /* Read SPIR-V files from disk and create shader modules from them */
  std::vector<uint32_t> vert_spv = ReadSpvFile(kVertPath);
  std::vector<uint32_t> frag_spv = ReadSpvFile(kFragPath);
  VkShaderModule vert_module = CreateShaderModule(vert_spv, device_, allocator_);
  VkShaderModule frag_module = CreateShaderModule(frag_spv, device_, allocator_);

  /* Perform reflection on the shaders to find the number and type of vertex input binding descriptions, etc. */
  ShaderReflectionDetails vert_reflection(vert_spv);
  ShaderReflectionDetails frag_reflection(frag_spv);

  /* Define the pipeline shader stages - vertex and fragment */
  VkPipelineShaderStageCreateInfo vert_stage_info { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
  vert_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vert_stage_info.module = vert_module;
  vert_stage_info.pName = "main";

  VkPipelineShaderStageCreateInfo frag_stage_info { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
  frag_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  frag_stage_info.module = frag_module;
  frag_stage_info.pName = "main";

  VkPipelineShaderStageCreateInfo shader_stages[] = { vert_stage_info, frag_stage_info };

  /* Vertex input descriptors */
  /* We'll use SPIRV-Cross to grab this information at some point */
  VkPipelineVertexInputStateCreateInfo vertex_input_info { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
  vertex_input_info.vertexBindingDescriptionCount   = 0;
  vertex_input_info.vertexAttributeDescriptionCount = 0;

  /* Input assembly */
  VkPipelineInputAssemblyStateCreateInfo input_assembly { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
  input_assembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly.primitiveRestartEnable = VK_FALSE;

  /* Dynamic viewport and scissor */
  /* Allegedly on most platforms there is negligible performance overhead incurred using dynamic viewport/scissor */
  /* However it would be nice to have the option to disable this for exclusive fullscreen applications */
  std::vector<VkDynamicState> dynamic_states = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
  };
  VkPipelineDynamicStateCreateInfo dynamic_state_info { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
  dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
  dynamic_state_info.pDynamicStates = dynamic_states.data();

  /* We only need to specify the number of viewports/scissors. These are defined  */
  VkPipelineViewportStateCreateInfo viewport_state { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
  viewport_state.viewportCount = 1;
  viewport_state.scissorCount  = 1;

  /* Rasterizer */
  VkPipelineRasterizationStateCreateInfo rasterization_state {
    VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
  rasterization_state.depthClampEnable        = VK_FALSE;
  rasterization_state.rasterizerDiscardEnable = VK_FALSE;
  rasterization_state.polygonMode             = VK_POLYGON_MODE_FILL;
  rasterization_state.lineWidth               = 1.0f;
  rasterization_state.cullMode                = VK_CULL_MODE_BACK_BIT;
  rasterization_state.frontFace               = VK_FRONT_FACE_CLOCKWISE;
  rasterization_state.depthBiasEnable         = VK_FALSE;

  /* Multisampling */
  VkPipelineMultisampleStateCreateInfo multisample_state { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
  multisample_state.sampleShadingEnable  = VK_FALSE;
  multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  /* Color blending */
  /* VkPipelineColorBlendAttachmentState specifies per-framebuffer color blend settings */
  VkPipelineColorBlendAttachmentState color_blend_attachment_state { };
  color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
                                              | VK_COLOR_COMPONENT_G_BIT
                                              | VK_COLOR_COMPONENT_B_BIT
                                              | VK_COLOR_COMPONENT_A_BIT;
  color_blend_attachment_state.blendEnable    = VK_FALSE;

  /* VkPipelineColorBlendStateCreateInfo specifies global color blend settings */
  VkPipelineColorBlendStateCreateInfo color_blend_state { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
  color_blend_state.logicOpEnable = VK_FALSE;
  color_blend_state.logicOp = VK_LOGIC_OP_COPY;
  color_blend_state.attachmentCount = 1;
  color_blend_state.pAttachments = &color_blend_attachment_state;

  /* Pipeline layout */
  /* This describes uniforms and push constants */
  VkPipelineLayoutCreateInfo pipeline_layout_info { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
  pipeline_layout_info.setLayoutCount         = 0;
  pipeline_layout_info.pSetLayouts            = nullptr;
  pipeline_layout_info.pushConstantRangeCount = 0;
  pipeline_layout_info.pPushConstantRanges    = nullptr;

  VK_CHECK(vkCreatePipelineLayout(device_, &pipeline_layout_info, allocator_, &graphics_pipeline_layout_));

  /* Create the pipeline */
  VkGraphicsPipelineCreateInfo pipeline_info { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
  pipeline_info.stageCount          = 2;
  pipeline_info.pStages             = shader_stages;
  pipeline_info.pVertexInputState   = &vertex_input_info;
  pipeline_info.pInputAssemblyState = &input_assembly;
  pipeline_info.pViewportState      = &viewport_state;
  pipeline_info.pRasterizationState = &rasterization_state;
  pipeline_info.pMultisampleState   = &multisample_state;
  pipeline_info.pDepthStencilState  = nullptr;
  pipeline_info.pColorBlendState    = &color_blend_state;
  pipeline_info.pDynamicState       = &dynamic_state_info;
  pipeline_info.layout              = graphics_pipeline_layout_;
  pipeline_info.renderPass          = render_pass_;
  pipeline_info.subpass             = 0;

  VK_CHECK(vkCreateGraphicsPipelines(device_, VK_NULL_HANDLE, 1, &pipeline_info, allocator_, &graphics_pipeline_));

  /* Cleanup the shader module objects */
  vkDestroyShaderModule(device_, vert_module, allocator_);
  vkDestroyShaderModule(device_, frag_module, allocator_);
}

void VulkanRenderer::DestroyGraphicsPipeline() {
  vkDestroyPipeline(device_, graphics_pipeline_, allocator_);
  vkDestroyPipelineLayout(device_, graphics_pipeline_layout_, allocator_);
}

}  // namespace Skeleton::Vulkan
