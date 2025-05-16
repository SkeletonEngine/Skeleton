// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/gui/scene_graph_panel.hpp"
#include "skeleton/core/core.hpp"

#include <string>
#include <imgui/imgui.h>
#include "skeleton/scene/components/components.hpp"
#include "skeleton/scene/entity.hpp"

namespace Skeleton {

static entt::entity selected_entity_      = entt::null;
static entt::entity marked_for_deletion_  = entt::null;
static entt::entity marked_for_new_child_ = entt::null;
static int id_ = 0;

entt::entity GetSelectedEntity() {
  return selected_entity_;
}

static void DeleteNode(entt::registry* scene, entt::entity entity) {
  // Cascade delete over all child nodes
  const auto& children = scene->get<ChildrenComponent>(entity).children;
  for (const auto& child : children) {
    DeleteNode(scene, child);
  }

  entt::entity parent = scene->get<ParentComponent>(entity).parent;
  auto& siblings = scene->get<ChildrenComponent>(parent).children;
  siblings.erase(std::remove(siblings.begin(), siblings.end(), entity), siblings.end());

  scene->destroy(entity);
}

static void DrawNode(entt::registry* scene, entt::entity entity, bool entity_is_root) {
  ImGui::PushID(id_++);

  const std::string& name = scene->get<NameComponent>(entity).name;
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen
                           | ImGuiTreeNodeFlags_OpenOnArrow
                           | ImGuiTreeNodeFlags_DrawLinesToNodes;
  if (entity == selected_entity_) {
    flags |= ImGuiTreeNodeFlags_Selected;
  }

  if (ImGui::TreeNodeEx(name.c_str(), flags)) {
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
      selected_entity_ = entity;
    }

    // Buttons to allow creating a new child node or deleting a node
    ImGui::SameLine();
    ImGui::Dummy({ ImGui::GetContentRegionAvail().x - 40, 0 });
    ImGui::SameLine();
    if (ImGui::Button("+")) {
      marked_for_new_child_ = entity;
    }

    if (!entity_is_root) {
      ImGui::SameLine();
      if (ImGui::Button("-")) {
        marked_for_deletion_ = entity;
      }
    }

    // Display all children of the current node
    const auto& children = scene->get<ChildrenComponent>(entity).children;
    for (const auto& child : children) {
      DrawNode(scene, child, false);
    }

    ImGui::TreePop();
  }

  ImGui::PopID();
}

void DrawSceneGraphPanel(entt::registry* scene, entt::entity root) {
  ImGui::Begin("Scene Graph");
  id_ = 0;
  DrawNode(scene, root, true);
  ImGui::End();

  if (marked_for_deletion_ != entt::null) {
    DeleteNode(scene, marked_for_deletion_);
    marked_for_deletion_ = entt::null;
  }

  if (marked_for_new_child_ != entt::null) {
    entt::entity new_entity = CreateEmptyEntity(scene, marked_for_new_child_, "New Entity");
    marked_for_new_child_ = entt::null;
  }
}

}  // namespace Skeleton
