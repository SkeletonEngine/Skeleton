// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/renderer/vulkan/vulkan_editor_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include "skeleton/window/glfw/glfw_window.hpp"
#include "skeleton/renderer/vulkan/vulkan_device_queue_families.hpp"

namespace Skeleton::Vulkan {

VulkanEditorRenderer::VulkanEditorRenderer(Window* window) : VulkanRenderer(window) {
  // The editor uses imgui, so we have to set it up
  // First, create the context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::GetIO().IniFilename = nullptr;

  // Init GLFW - for this we need the GLFWwindow*
  GlfwWindow* glfw_window = dynamic_cast<GlfwWindow*>(window);
  ImGui_ImplGlfw_InitForVulkan(glfw_window->GetNativeWindowHandle(), true);

  // Init Vulkan - for this we need a whole load of Vulkan objects
  // Imgui needs its own descriptor pool
  VkDescriptorPoolSize pool_sizes[] = {
    { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE },
  };
  VkDescriptorPoolCreateInfo pool_info = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
  pool_info.flags   = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  pool_info.maxSets = 0;
  for (VkDescriptorPoolSize& pool_size : pool_sizes) {
    pool_info.maxSets += pool_size.descriptorCount;
  }
  pool_info.poolSizeCount = static_cast<uint32_t>(IM_ARRAYSIZE(pool_sizes));
  pool_info.pPoolSizes    = pool_sizes;
  VK_CHECK(vkCreateDescriptorPool(device_, &pool_info, allocator_, &imgui_descriptor_pool_));

  DeviceQueueFamilies queue_families(physical_device_, surface_);

  ImGui_ImplVulkan_InitInfo init_info = { };
  init_info.ApiVersion      = VK_API_VERSION_1_0;
  init_info.Instance        = instance_;
  init_info.PhysicalDevice  = physical_device_;
  init_info.Device          = device_;
  init_info.QueueFamily     = queue_families.GraphicsFamilyIndex();
  init_info.Queue           = graphics_queue_;
  init_info.PipelineCache   = VK_NULL_HANDLE;
  init_info.DescriptorPool  = imgui_descriptor_pool_;
  init_info.RenderPass      = render_pass_;
  init_info.Subpass         = 0;
  init_info.MinImageCount   = swapchain_min_image_count_;
  init_info.ImageCount      = swapchain_images_.size();
  init_info.MSAASamples     = VK_SAMPLE_COUNT_1_BIT;
  init_info.Allocator       = allocator_;
  init_info.CheckVkResultFn = [](VkResult r) { VK_CHECK(r); };
  ImGui_ImplVulkan_Init(&init_info);
}

VulkanEditorRenderer::~VulkanEditorRenderer() {
  vkDeviceWaitIdle(device_);

  ImGui_ImplVulkan_Shutdown();
  vkDestroyDescriptorPool(device_, imgui_descriptor_pool_, allocator_);
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void VulkanEditorRenderer::RenderFrame() {
  // If the window was minimized, don't render anything
  if (window_minimized_) {
    return;
  }

  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::ShowDemoWindow();

  ImGui::Render();

  BeginFrame();
  BeginRenderCommandBuffer();

  RenderMesh();

  ImDrawData* draw_data = ImGui::GetDrawData();
  ImGui_ImplVulkan_RenderDrawData(draw_data, render_command_buffers_[current_frame_]);

  EndRenderCommandBuffer();
  EndFrame();
}

}  // namespace Skeleton::Vulkan
