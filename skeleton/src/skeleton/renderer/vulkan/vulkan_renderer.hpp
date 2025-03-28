// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <vector>
#include <volk.h>
#include "skeleton/application_settings.hpp"
#include "skeleton/renderer/renderer.hpp"
#include "skeleton/window/window.hpp"

namespace Skeleton::Vulkan {

class VulkanRenderer : public Renderer {
 public:
  VulkanRenderer(const ApplicationSettings& settings, Window* window);
  virtual ~VulkanRenderer();

 public:
  virtual void RenderFrame() override;

 private:
  void CreateInstance();
  void DestroyInstance();
  void CreateWindowSurface();
  void DestroyWindowSurface();
  void ChoosePhysicalDevice();
  void CreateDevice();
  void DestroyDevice();
  void CreateSwapchain();
  void DestroySwapchain();
  void CreateSwapchainImageViews();
  void DestroySwapchainImageViews();
  void CreateRenderPass();
  void DestroyRenderPass();
  void CreateGraphicsPipeline();
  void DestroyGraphicsPipeline();
  void CreateSwapchainFramebuffers();
  void DestroySwapchainFramebuffers();
  void CreateCommandPool();
  void DestroyCommandPool();
  void CreateRenderCommandBuffer();
  void DestroyRenderCommandBuffer();
  void CreateSyncObjects();
  void DestroySyncObjects();

private:
  void RecordRenderCommandBuffer(VkCommandBuffer command_buffer, uint32_t image_index);

 private:
  /* Non-owning pointer to the window */
  Window* window_;

 private:
  /* Objects owned by the renderer */
  VkAllocationCallbacks*   allocator_       = VK_NULL_HANDLE;
  VkInstance               instance_        = VK_NULL_HANDLE;
  VkPhysicalDevice         physical_device_ = VK_NULL_HANDLE;
  VkDevice                 device_          = VK_NULL_HANDLE;
  VkQueue                  graphics_queue_  = VK_NULL_HANDLE;
  VkQueue                  present_queue_   = VK_NULL_HANDLE;
  VkSurfaceKHR             surface_         = VK_NULL_HANDLE;
  VkSwapchainKHR           swapchain_       = VK_NULL_HANDLE;
  std::vector<VkImage>     swapchain_images_; 
  VkExtent2D               swapchain_extent_;
  VkFormat                 swapchain_image_format_;
  std::vector<VkImageView> swapchain_image_views_;
  std::vector<VkFramebuffer> swapchain_framebuffers_;
  bool                     vsync_           = true;
  VkRenderPass             render_pass_     = VK_NULL_HANDLE;
  VkPipeline               graphics_pipeline_ = VK_NULL_HANDLE;
  VkPipelineLayout         graphics_pipeline_layout_ = VK_NULL_HANDLE;
  VkCommandPool            command_pool_    = VK_NULL_HANDLE;
  VkCommandBuffer          render_command_buffer_ = VK_NULL_HANDLE;
  VkSemaphore              image_available_semaphore_ = VK_NULL_HANDLE;
  VkSemaphore              render_complete_semaphore_ = VK_NULL_HANDLE;
  VkFence                  in_flight_fence_           = VK_NULL_HANDLE;
};

}  // namespace Skeleton::Vulkan
