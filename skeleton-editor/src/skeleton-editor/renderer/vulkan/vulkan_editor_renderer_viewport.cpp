// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/renderer/vulkan/vulkan_editor_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <imgui/imgui.h>

namespace Skeleton::Vulkan {

void VulkanEditorRenderer::DrawViewport() {
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
}

}  // namespace Skeleton::Vulkan
