// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/gui/dockspace.hpp"
#include "skeleton/core/core.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Skeleton {

void DrawDockspace() {
  ImGuiID dockspace = ImGui::DockSpaceOverViewport();
  static bool dockspace_setup = false;
  if (!dockspace_setup) {
    ImGuiID viewport = ImGui::DockBuilderAddNode(dockspace, 0);
    ImGui::DockBuilderDockWindow("Viewport", viewport);
    dockspace_setup = true;
  }
}

}  // namespace Skeleton
