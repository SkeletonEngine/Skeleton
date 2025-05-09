// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/gui/scene_graph_panel.hpp"
#include "skeleton/core/core.hpp"

#include <string>
#include <imgui/imgui.h>
#include "skeleton/scene/components/components.hpp"

namespace Skeleton {

static void DrawNode(entt::registry* scene, entt::entity entity) {
  const std::string& name = scene->get<NameComponent>(entity).name;
  if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_DrawLinesToNodes)) {
    // Display ClearColorComponent if it exists
    // TODO(jack): Move into inspector panel
    if (scene->any_of<ClearColorComponent>(entity)) {
      auto& clear_color = scene->get<ClearColorComponent>(entity).color;
      ImGui::ColorEdit4("Clear Color", clear_color.rgba);
    }

    // Display all children of the current node
    const auto& children = scene->get<ChildrenComponent>(entity).children;
    for (const auto& child : children) {
      DrawNode(scene, child);
    }

    ImGui::TreePop();
  }
}

void DrawSceneGraphPanel(entt::registry* scene, entt::entity root) {
  ImGui::Begin("Scene Graph");

  DrawNode(scene, root);

  ImGui::End();
}

}  // namespace Skeleton
