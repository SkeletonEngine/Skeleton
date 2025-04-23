// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/renderer/vulkan/vulkan_editor_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include "skeleton/window/glfw/glfw_window.hpp"
#include "skeleton/renderer/vulkan/vulkan_device_queue_families.hpp"

namespace Skeleton::Vulkan {

VulkanEditorRenderer::VulkanEditorRenderer(const RendererSettings& settings)
    : VulkanRenderer(settings, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
  // The editor uses imgui, so we have to set it up
  // First, create the context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::GetIO().IniFilename = nullptr;

  // Init Imgui with GLFW - Imgui needs the GLFWwindow*
  GlfwWindow* glfw_window = dynamic_cast<GlfwWindow*>(settings.window);
  ImGui_ImplGlfw_InitForVulkan(glfw_window->GetNativeWindowHandle(), true);

  // Create necessary resources for Imgui - a descriptor pool and a render pass
  CreateImguiDescriptorPool();
  CreateImguiRenderPass();

  // Set the scene render pass to render to the viewport framebuffers
  render_target_framebuffers_ = &editor_viewport_framebuffers_;
  render_target_extent_       = &editor_viewport_extent_;

  // Init Imgui with Vulkan backend - Imgui needs a whole load of Vulkan objects
  // It has its own descriptor pool and render pass, other than that we can pass
  // the instance/device/queues/etc. that we already have
  ImGui_ImplVulkan_InitInfo init_info = { };
  init_info.ApiVersion      = SK_VK_API_VERSION;
  init_info.Instance        = instance_;
  init_info.PhysicalDevice  = physical_device_;
  init_info.Device          = device_;
  init_info.QueueFamily     = DeviceQueueFamilies(physical_device_, surface_).GraphicsFamilyIndex();
  init_info.Queue           = graphics_queue_;
  init_info.PipelineCache   = VK_NULL_HANDLE;
  init_info.DescriptorPool  = imgui_descriptor_pool_;
  init_info.RenderPass      = imgui_render_pass_;
  init_info.Subpass         = 0;
  init_info.MinImageCount   = swapchain_min_image_count_;
  init_info.ImageCount      = swapchain_images_.size();
  init_info.MSAASamples     = VK_SAMPLE_COUNT_1_BIT;
  init_info.Allocator       = allocator_;
  init_info.CheckVkResultFn = [](VkResult r) { VK_CHECK(r); };
  ImGui_ImplVulkan_Init(&init_info);

  // We need to init Imgui for Vulkan before creating the framebuffers so that Imgui can allocate descriptor sets
  editor_viewport_images_.resize(swapchain_images_.size());
  editor_viewport_image_allocations_.resize(swapchain_images_.size());
  editor_viewport_image_views_.resize(swapchain_images_.size());
  editor_viewport_samplers_.resize(swapchain_images_.size());
  editor_viewport_descriptor_sets_.resize(swapchain_images_.size());
  editor_viewport_framebuffers_.resize(swapchain_images_.size());
  editor_viewport_framebuffers_dirty_.resize(swapchain_images_.size(), true);
  for (size_t i = 0; i < swapchain_images_.size(); ++i) {
    CreateViewportFramebuffer(i);
  }
}

VulkanEditorRenderer::~VulkanEditorRenderer() {
  // Wait for frames to finish rendering before allowing imgui to destroy its Vulkan objects
  vkDeviceWaitIdle(device_);

  // Destroy Imgui Vulkan objects in reverse order of creation
  for (size_t i = 0; i < swapchain_images_.size(); ++i) {
    DestroyViewportFramebuffer(i);
  }
  ImGui_ImplVulkan_Shutdown();
  DestroyImguiRenderPass();
  DestroyImguiDescriptorPool();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void VulkanEditorRenderer::RenderFrame() {
  // If the window was minimized, don't render anything
  if (window_minimized_) {
    return;
  }

  // Render Imgui
  // None of this actually touches the GPU, it just creates vertex
  // buffers, etc. to be used later by ImGui_ImplVulkan_RenderDrawData
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::ShowDemoWindow();

  // Blit the scene framebuffer to an Imgui window
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
  ImGui::Begin("Viewport");
  // Keep track of the previous size of the window so we can resize the framebuffers when needed
  ImVec2 size = ImGui::GetContentRegionAvail();
  // If the size has changed, mark the framebuffers to be resized and projection matrices to be recalculated
  if (editor_viewport_extent_.width != size.x || editor_viewport_extent_.height != size.y) {
    editor_viewport_extent_.width  = static_cast<uint32_t>(size.x);
    editor_viewport_extent_.height = static_cast<uint32_t>(size.y);
    editor_viewport_minimized_ = editor_viewport_extent_.width == 0 || editor_viewport_extent_.height == 0;
    if (!editor_viewport_minimized_) {
      for (size_t i = 0; i < swapchain_images_.size(); ++i) {
        editor_viewport_framebuffers_dirty_[i] = true;
      }
      for (uint32_t i = 0; i < kMaxFramesInFlight; ++i) {
        projection_matrix_dirty_[i] = true;
      }
    }
  }
  if (!editor_viewport_framebuffers_dirty_[image_index_]) {
    ImGui::Image((ImTextureID)editor_viewport_descriptor_sets_[image_index_], size);
  }
  ImGui::End();
  ImGui::PopStyleVar();

  ImGui::Render();

  // Now we perform the actual Vulkan commands
  BeginFrame();

  // BeginFrame performs synchronization, so we are now safe to delete and
  // recreate framebuffers if they are the wrong size for the Imgui window
  if (editor_viewport_framebuffers_dirty_[image_index_]) {
    DestroyViewportFramebuffer(image_index_);
    CreateViewportFramebuffer(image_index_);
    editor_viewport_framebuffers_dirty_[image_index_] = false;
  }

  BeginRenderCommandBuffer();
  if (!editor_viewport_minimized_) {
    PerformSceneRenderPass();
  }
  PerformImguiRenderPass();
  EndRenderCommandBuffer();
  EndFrame();
}

void VulkanEditorRenderer::PerformImguiRenderPass() {
  // Begin the render pass
  VkRenderPassBeginInfo render_pass_info { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
  render_pass_info.renderPass  = imgui_render_pass_;
  render_pass_info.framebuffer = swapchain_framebuffers_[image_index_];
  render_pass_info.renderArea.offset = { 0, 0 };
  render_pass_info.renderArea.extent = swapchain_extent_;
  VkClearValue clear_color = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}};
  render_pass_info.clearValueCount = 1;
  render_pass_info.pClearValues = &clear_color;
  vkCmdBeginRenderPass(render_command_buffers_[current_frame_], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

  // Bind the pipeline
  vkCmdBindPipeline(render_command_buffers_[current_frame_], VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline_);

  // Set dynamic viewport and scissor
  VkViewport viewport { };
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapchain_extent_.width);
  viewport.height = static_cast<float>(swapchain_extent_.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(render_command_buffers_[current_frame_], 0, 1, &viewport);

  VkRect2D scissor { };
  scissor.offset = { 0, 0 };
  scissor.extent = swapchain_extent_;
  vkCmdSetScissor(render_command_buffers_[current_frame_], 0, 1, &scissor);

  // Bind the vertex buffer
  VkBuffer vertex_buffers[] = { vertex_buffer_ };
  VkDeviceSize offsets[] = { 0 };
  vkCmdBindVertexBuffers(render_command_buffers_[current_frame_], 0, 1, vertex_buffers, offsets);

  // Tell imgui to actually do the Vulkan stuff it needs to do to render the gui
  ImDrawData* draw_data = ImGui::GetDrawData();
  ImGui_ImplVulkan_RenderDrawData(draw_data, render_command_buffers_[current_frame_]);

  // End the render pass
  vkCmdEndRenderPass(render_command_buffers_[current_frame_]);
}

}  // namespace Skeleton::Vulkan
