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
    // Create the initial layout
    ImGuiID viewport = ImGui::DockBuilderAddNode(dockspace, 0);
    ImGuiID scene_graph_panel = ImGui::DockBuilderSplitNode(
      viewport, ImGuiDir_Left, 0.25f, &scene_graph_panel, &viewport);
    ImGuiID entity_properties_panel = ImGui::DockBuilderSplitNode(
      scene_graph_panel, ImGuiDir_Down, 0.5f, &entity_properties_panel, &scene_graph_panel);

    // Dock the viewport and auto-hide its tab bar
    ImGui::DockBuilderDockWindow("Viewport", viewport);
    ImGuiDockNode* viewport_node = ImGui::DockBuilderGetNode(viewport);
    viewport_node->LocalFlags |= ImGuiDockNodeFlags_AutoHideTabBar;

    // Dock the other windows
    ImGui::DockBuilderDockWindow("Scene Graph", scene_graph_panel);
    ImGui::DockBuilderDockWindow("Entity Properties", entity_properties_panel);

    // Dockspace setup is now complete, set the latch to ensure this only ever happens once
    dockspace_setup = true;
  }
}

}  // namespace Skeleton
