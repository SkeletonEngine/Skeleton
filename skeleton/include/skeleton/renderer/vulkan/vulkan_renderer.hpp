// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/renderer/vulkan/vulkan_core.hpp"

#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include "skeleton/renderer/renderer.hpp"
#include "skeleton/renderer/renderer_settings.hpp"
#include "skeleton/window/window.hpp"

namespace Skeleton::Vulkan {

class VulkanRenderer : public Renderer {
 public:
  explicit VulkanRenderer(const RendererSettings& settings, VkImageLayout final_layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
  virtual ~VulkanRenderer();

 public:
  virtual RendererBackend GetBackend() const override { return RendererBackend::kVulkan; }

 public:
  virtual void RenderFrame() override;

 protected:
  void BeginFrame();
  void EndFrame();

 public:
  virtual std::string GetRendererString() const override;

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
  void RecreateSwapchain();
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
  void CreateVmaAllocator();
  void DestroyVmaAllocator();
  void CreateMesh();
  void DestroyMesh();

 private:
#ifdef SK_BUILD_DEBUG
  void CreateDebugMessenger();
  void DestroyDebugMessenger();
  void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* to_populate);
#endif  // SK_BUILD_DEBUG

 protected:
  void BeginRenderCommandBuffer();
  void EndRenderCommandBuffer();
  void PerformSceneRenderPass();

 private:
  VkCommandBuffer BeginSingleUseCommandBuffer();
  void EndSingleUseCommandBuffer(VkCommandBuffer command_buffer);

 private:
  void CreateDeviceLocalBuffer(const void* data, VkDeviceSize size, VkBuffer* buffer,
                               VmaAllocation* allocation, VkBufferUsageFlagBits usage);

 protected:
  /* Constants */
  const uint32_t kMaxFramesInFlight = 2;

  enum {
    kUboBindingCameraMatrix = 0,
    kUboBindingModelMatrix  = 1,
  };

 private:
  /* Non-owning pointer to the window */
  Window* window_;

 protected:
  /* Objects owned by the renderer */
  VkAllocationCallbacks*   allocator_       = VK_NULL_HANDLE;
  VmaAllocator             vma_allocator_   = VK_NULL_HANDLE;
  VkInstance               instance_        = VK_NULL_HANDLE;
  VkPhysicalDevice         physical_device_ = VK_NULL_HANDLE;
  VkDevice                 device_          = VK_NULL_HANDLE;
  VkQueue                  graphics_queue_  = VK_NULL_HANDLE;
  VkQueue                  present_queue_   = VK_NULL_HANDLE;
  VkSurfaceKHR             surface_         = VK_NULL_HANDLE;
  VkSwapchainKHR           swapchain_       = VK_NULL_HANDLE;
  std::vector<VkImage>     swapchain_images_;
  uint32_t                 swapchain_min_image_count_;
  VkExtent2D               swapchain_extent_;
  VkFormat                 swapchain_image_format_;
  std::vector<VkImageView> swapchain_image_views_;
  std::vector<VkFramebuffer> swapchain_framebuffers_;
  bool                     vsync_           = true;
  VkRenderPass             render_pass_     = VK_NULL_HANDLE;
  VkImageLayout            final_image_layout_;
  VkCommandPool            command_pool_    = VK_NULL_HANDLE;
  std::vector<VkCommandBuffer> render_command_buffers_;
  std::vector<VkSemaphore> image_available_semaphores_;
  std::vector<VkSemaphore> render_complete_semaphores_;
  std::vector<VkFence>     in_flight_fences_;
  bool                     window_framebuffer_resized_ = false;
  bool                     window_minimized_           = false;
  std::vector<bool>        projection_matrix_dirty_;
  VkBuffer                 vertex_buffer_   = VK_NULL_HANDLE;
  VmaAllocation            vertex_buffer_allocation_ = VK_NULL_HANDLE;
  VkBuffer                 index_buffer_   = VK_NULL_HANDLE;
  VmaAllocation            index_buffer_allocation_ = VK_NULL_HANDLE;
  uint32_t                 index_count_    = 0;

  // Per-pipeline objects
  VkPipeline                   graphics_pipeline_ = VK_NULL_HANDLE;
  VkPipelineLayout             graphics_pipeline_layout_ = VK_NULL_HANDLE;
  VkDescriptorSetLayout        descriptor_set_layout_ = VK_NULL_HANDLE;
  VkDescriptorPool             descriptor_pool_;
  std::vector<VkDescriptorSet> descriptor_sets_;
  struct UniformBuffer {
    std::vector<VkBuffer>        buffers;
    std::vector<VmaAllocation>   allocations;
    std::vector<void*>           mapped_memory;
    uint32_t                     size = 0;
  };
  std::unordered_map<uint32_t, UniformBuffer>   uniform_buffers_;

  // Transient, keeps track of the acquired image indicex and the current frame-in-flight during rendering a frame
  uint32_t image_index_   = 0;
  uint32_t current_frame_ = 0;

 protected:
  // Derived classes can alter these to render the final scene to a texture instead of directly to the swapchain
  std::vector<VkFramebuffer>* render_target_framebuffers_ = &swapchain_framebuffers_;
  VkExtent2D*                 render_target_extent_       = &swapchain_extent_;

 private:
#ifdef SK_BUILD_DEBUG
  VkDebugUtilsMessengerEXT debug_messenger_ = VK_NULL_HANDLE;
#endif  // SK_BUILD_DEBUG
};

}  // namespace Skeleton::Vulkan
