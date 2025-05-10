// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/gui/scene_graph_panel.hpp"
#include "skeleton/core/core.hpp"

#include <string>
#include <imgui/imgui.h>
#include "skeleton/scene/components/components.hpp"
#include "skeleton-editor/gui/scene_graph_panel.hpp"

namespace Skeleton {

void DrawEntityPropertiesPanel(entt::registry* scene) {
  entt::entity entity = GetSelectedEntity();

  ImGui::Begin("Entity Properties");
  if (entity == entt::null) {
    ImGui::End();
    return;
  }

  // Display NameComponent
  auto& name = scene->get<NameComponent>(entity).name;
  ImGui::Text("%s", name.c_str());
  ImGui::Separator();

  // Display ClearColorComponent if it exists
  if (scene->any_of<ClearColorComponent>(entity)) {
    auto& clear_color = scene->get<ClearColorComponent>(entity).color;
    ImGui::ColorEdit4("Clear Color", clear_color.rgba);
  }

  ImGui::End();
}

}  // namespace Skeleton
