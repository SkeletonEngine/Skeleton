// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/vulkan/vulkan_renderer.hpp"
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

#include <vector>
#include "skeleton/renderer/common/shader_reflection/shader_reflection_details.hpp"
#include "skeleton/renderer/common/spv_file.hpp"

namespace Skeleton::Vulkan {

static VkFormat DeduceFormat(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::kFloat:  return VK_FORMAT_R32_SFLOAT;
    case ShaderDataType::kFloat2: return VK_FORMAT_R32G32_SFLOAT;
    case ShaderDataType::kFloat3: return VK_FORMAT_R32G32B32_SFLOAT;
    case ShaderDataType::kFloat4: return VK_FORMAT_R32G32B32A32_SFLOAT;
    default: return VK_FORMAT_MAX_ENUM;
  }
}

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
  // Hard code the shader paths for testing
  const char* kVertPath = "build/shaders/test.vert.spv";
  const char* kFragPath = "build/shaders/test.frag.spv";

  // Read SPIR-V files from disk and create shader modules from them
  std::vector<uint32_t> vert_spv = ReadSpvFile(kVertPath);
  std::vector<uint32_t> frag_spv = ReadSpvFile(kFragPath);
  VkShaderModule vert_module = CreateShaderModule(vert_spv, device_, allocator_);
  VkShaderModule frag_module = CreateShaderModule(frag_spv, device_, allocator_);


  // Define the pipeline shader stages - vertex and fragment
  VkPipelineShaderStageCreateInfo vert_stage_info { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
  vert_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vert_stage_info.module = vert_module;
  vert_stage_info.pName = "main";

  VkPipelineShaderStageCreateInfo frag_stage_info { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
  frag_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  frag_stage_info.module = frag_module;
  frag_stage_info.pName = "main";

  VkPipelineShaderStageCreateInfo shader_stages[] = { vert_stage_info, frag_stage_info };

  // Vertex input descriptors
  // Use SPIRV-Cross to perform reflection on the shaders
  // Find the number and type of vertex inputs, and the number, size and binding of any uniform buffers
  ShaderReflectionDetails vert_reflection(vert_spv);
  ShaderReflectionDetails frag_reflection(frag_spv);

  // We only need one vertex input binding since our vertex data is interleaved in a single array
  VkVertexInputBindingDescription vertex_binding_description { };
  vertex_binding_description.binding = 0;
  vertex_binding_description.stride = vert_reflection.vertex_input_layout.GetSize();
  vertex_binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

  // We need one attribute description for each layout (location = xyz) in our vertex shader
  std::vector<VkVertexInputAttributeDescription> vertex_input_attribs;
  for (const auto& element : vert_reflection.vertex_input_layout) {
    VkVertexInputAttributeDescription attrib { };
    attrib.binding = 0;
    attrib.location = element.location.value();
    attrib.format = DeduceFormat(element.type);
    attrib.offset = element.offset;
    vertex_input_attribs.push_back(attrib);
  }

  VkPipelineVertexInputStateCreateInfo vertex_input_info { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
  vertex_input_info.vertexBindingDescriptionCount   = 1;
  vertex_input_info.pVertexBindingDescriptions      = &vertex_binding_description;
  vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertex_input_attribs.size());
  vertex_input_info.pVertexAttributeDescriptions    = vertex_input_attribs.data();

  // Input assembly
  VkPipelineInputAssemblyStateCreateInfo input_assembly { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
  input_assembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly.primitiveRestartEnable = VK_FALSE;

  // Dynamic viewport and scissor
  // Allegedly on most platforms there is negligible performance overhead incurred using dynamic viewport/scissor
  // However it would be nice to have the option to disable this for exclusive fullscreen applications
  std::vector<VkDynamicState> dynamic_states = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
  };
  VkPipelineDynamicStateCreateInfo dynamic_state_info { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
  dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
  dynamic_state_info.pDynamicStates = dynamic_states.data();

  // We only need to specify the number of viewports/scissors. These are defined
  // later when recording the command buffer
  VkPipelineViewportStateCreateInfo viewport_state { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
  viewport_state.viewportCount = 1;
  viewport_state.scissorCount  = 1;

  // Rasterizer
  VkPipelineRasterizationStateCreateInfo rasterization_state {
    VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
  rasterization_state.depthClampEnable        = VK_FALSE;
  rasterization_state.rasterizerDiscardEnable = VK_FALSE;
  rasterization_state.polygonMode             = VK_POLYGON_MODE_FILL;
  rasterization_state.lineWidth               = 1.0f;
  rasterization_state.cullMode                = VK_CULL_MODE_BACK_BIT;
  rasterization_state.frontFace               = VK_FRONT_FACE_CLOCKWISE;
  rasterization_state.depthBiasEnable         = VK_FALSE;

  // Multisampling
  VkPipelineMultisampleStateCreateInfo multisample_state { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
  multisample_state.sampleShadingEnable  = VK_FALSE;
  multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  // Color blending
  // VkPipelineColorBlendAttachmentState specifies per-framebuffer color blend settings
  VkPipelineColorBlendAttachmentState color_blend_attachment_state { };
  color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
                                              | VK_COLOR_COMPONENT_G_BIT
                                              | VK_COLOR_COMPONENT_B_BIT
                                              | VK_COLOR_COMPONENT_A_BIT;
  color_blend_attachment_state.blendEnable    = VK_FALSE;

  // VkPipelineColorBlendStateCreateInfo specifies global color blend settings
  VkPipelineColorBlendStateCreateInfo color_blend_state { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
  color_blend_state.logicOpEnable   = VK_FALSE;
  color_blend_state.logicOp         = VK_LOGIC_OP_COPY;
  color_blend_state.attachmentCount = 1;
  color_blend_state.pAttachments    = &color_blend_attachment_state;

  // Descriptor set layout
  // Perform reflection on the shaders to find all uniform buffers present and create a layout binding for each one
  std::vector<VkDescriptorSetLayoutBinding> layout_bindings;

  auto add_layout_binding = [&](uint32_t binding) {
    VkDescriptorSetLayoutBinding layout_binding { };
    layout_binding.binding         = binding;
    layout_binding.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layout_binding.descriptorCount = 1;
    layout_binding.stageFlags      = VK_SHADER_STAGE_VERTEX_BIT;
    layout_bindings.push_back(layout_binding);
  };

  for (auto& ubo : vert_reflection.uniform_buffers) {
    add_layout_binding(ubo.first);
  }
  for (auto& ubo : frag_reflection.uniform_buffers) {
    add_layout_binding(ubo.first);
  }

  // Create the descriptor set layout
  VkDescriptorSetLayoutCreateInfo layout_info { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
  layout_info.bindingCount = static_cast<uint32_t>(layout_bindings.size());
  layout_info.pBindings    = layout_bindings.data();
  vkCreateDescriptorSetLayout(device_, &layout_info, allocator_, &descriptor_set_layout_);

  // Pipeline layout
  // This describes uniforms and push constants
  VkPipelineLayoutCreateInfo pipeline_layout_info { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
  pipeline_layout_info.setLayoutCount         = 1;
  pipeline_layout_info.pSetLayouts            = &descriptor_set_layout_;
  pipeline_layout_info.pushConstantRangeCount = 0;
  pipeline_layout_info.pPushConstantRanges    = nullptr;

  VK_CHECK(vkCreatePipelineLayout(device_, &pipeline_layout_info, allocator_, &graphics_pipeline_layout_));

  // Create the pipeline
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

  // Cleanup the shader module objects
  vkDestroyShaderModule(device_, vert_module, allocator_);
  vkDestroyShaderModule(device_, frag_module, allocator_);

  // If there are no uniform buffers to allocate then we don't need to allocate descriptors, so we're done
  if (vert_reflection.uniform_buffers.empty() && frag_reflection.uniform_buffers.empty()) {
    return;
  }

  // Allocate uniform buffers and map the memory
  auto create_uniform_buffer = [&](uint32_t binding, const ShaderBufferLayout& layout) {
    UniformBuffer ub;

    ub.buffers.resize(kMaxFramesInFlight);
    ub.allocations.resize(kMaxFramesInFlight);
    ub.mapped_memory.resize(kMaxFramesInFlight);
    ub.size = layout.GetSize();

    VkBufferCreateInfo buffer_info { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
    buffer_info.size        = layout.GetSize();
    buffer_info.usage       = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo alloc_create_info { };
    alloc_create_info.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    alloc_create_info.usage = VMA_MEMORY_USAGE_AUTO;
    alloc_create_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;

    VmaAllocationInfo alloc_info;

    for (uint32_t i = 0; i < kMaxFramesInFlight; ++i) {
      VK_CHECK(vmaCreateBuffer(vma_allocator_, &buffer_info, &alloc_create_info,
                               &ub.buffers[i], &ub.allocations[i], &alloc_info));
      ub.mapped_memory[i] = alloc_info.pMappedData;
    }

    uniform_buffers_.emplace(binding, ub);
  };

  for (auto& uniform_buffer : vert_reflection.uniform_buffers) {
    create_uniform_buffer(uniform_buffer.first, uniform_buffer.second);
  }
  for (auto& uniform_buffer : frag_reflection.uniform_buffers) {
    create_uniform_buffer(uniform_buffer.first, uniform_buffer.second);
  }

  // Create the descriptor pool for the uniform buffers
  VkDescriptorPoolSize pool_size { };
  pool_size.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  pool_size.descriptorCount = static_cast<uint32_t>(kMaxFramesInFlight * uniform_buffers_.size());

  VkDescriptorPoolCreateInfo pool_info { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
  pool_info.poolSizeCount = 1;
  pool_info.pPoolSizes    = &pool_size;
  pool_info.maxSets       = static_cast<uint32_t>(kMaxFramesInFlight * uniform_buffers_.size());

  VK_CHECK(vkCreateDescriptorPool(device_, &pool_info, allocator_, &descriptor_pool_));

  // Allocate descriptor sets from descriptor pool
  std::vector<VkDescriptorSetLayout> layouts(kMaxFramesInFlight, descriptor_set_layout_);
  VkDescriptorSetAllocateInfo alloc_info { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
  alloc_info.descriptorPool     = descriptor_pool_;
  alloc_info.descriptorSetCount = static_cast<uint32_t>(kMaxFramesInFlight * uniform_buffers_.size());
  alloc_info.pSetLayouts        = layouts.data();

  for (auto& ub : uniform_buffers_) {
    ub.second.descriptor_sets.resize(kMaxFramesInFlight);
    VK_CHECK(vkAllocateDescriptorSets(device_, &alloc_info, ub.second.descriptor_sets.data()));
  }

  // Configure the descriptor sets
  auto configure_descriptor_set = [&](uint32_t binding, const ShaderBufferLayout& layout) {
    for (uint32_t i = 0; i < kMaxFramesInFlight; ++i) {
      VkDescriptorBufferInfo buffer_info { };
      buffer_info.buffer = uniform_buffers_[binding].buffers[i];
      buffer_info.offset = 0;
      buffer_info.range = layout.GetSize();

      VkWriteDescriptorSet descriptor_write { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
      descriptor_write.dstSet          = uniform_buffers_[binding].descriptor_sets[i];
      descriptor_write.dstBinding      = binding;
      descriptor_write.dstArrayElement = 0;
      descriptor_write.descriptorCount = 1;
      descriptor_write.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      descriptor_write.pBufferInfo     = &buffer_info;

      vkUpdateDescriptorSets(device_, 1, &descriptor_write, 0, nullptr);
    }
  };

  for (auto& ubo : vert_reflection.uniform_buffers) {
    configure_descriptor_set(ubo.first, ubo.second);
  }
  for (auto& ubo : frag_reflection.uniform_buffers) {
    configure_descriptor_set(ubo.first, ubo.second);
  }
}

void VulkanRenderer::DestroyGraphicsPipeline() {
  if (!uniform_buffers_.empty()) {
    vkDestroyDescriptorPool(device_, descriptor_pool_, allocator_);
  }

  for (auto& ub : uniform_buffers_) {
    for (uint32_t i = 0; i < kMaxFramesInFlight; ++i) {
      vmaDestroyBuffer(vma_allocator_, ub.second.buffers[i], ub.second.allocations[i]);
    }
  }

  vkDestroyDescriptorSetLayout(device_, descriptor_set_layout_, allocator_);
  vkDestroyPipeline(device_, graphics_pipeline_, allocator_);
  vkDestroyPipelineLayout(device_, graphics_pipeline_layout_, allocator_);
}

}  // namespace Skeleton::Vulkan
