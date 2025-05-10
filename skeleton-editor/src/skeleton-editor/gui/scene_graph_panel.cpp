// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/gui/scene_graph_panel.hpp"
#include "skeleton/core/core.hpp"

#include <string>
#include <imgui/imgui.h>
#include "skeleton/scene/components/components.hpp"

namespace Skeleton {

static entt::entity selected_entity_ = entt::null;

entt::entity GetSelectedEntity() {
  return selected_entity_;
}

static void DrawNode(entt::registry* scene, entt::entity entity) {
  const std::string& name = scene->get<NameComponent>(entity).name;
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DrawLinesToNodes;
  if (entity == selected_entity_) {
    flags |= ImGuiTreeNodeFlags_Selected;
  }

  if (ImGui::TreeNodeEx(name.c_str(), flags)) {      
    if (ImGui::IsItemClicked()) {
      selected_entity_ = entity;
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
