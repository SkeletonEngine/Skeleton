// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/gui/entity_properties_panel.hpp"
#include "skeleton/core/core.hpp"

#include <string>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include "skeleton/scene/components/components.hpp"
#include "skeleton-editor/common/editor_preferences.hpp"
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
  ImGui::InputText("Name", &name);

  // Display UuidComponent
  auto& uuid = scene->get<UuidComponent>(entity).uuid;
  ImGui::TextDisabled("%s", uuids::to_string(uuid).c_str());
  ImGui::Separator();

  // Display ClearColorComponent if it exists
  if (scene->any_of<ClearColorComponent>(entity)) {
    auto& clear_color = scene->get<ClearColorComponent>(entity).color;
    ImGui::ColorEdit4("Clear Color", clear_color.rgba);
  }

  // Display CameraComponent if it exists
  if (scene->any_of<CameraComponent>(entity)) {
    auto& camera = scene->get<CameraComponent>(entity);

    // Display FOV in either degrees or radians
    const int kMinFovDegrees = 30;
    const int kMaxFovDegrees = 150;
    const float kMinFovRadians = glm::radians(static_cast<float>(kMinFovDegrees));
    const float kMaxFovRadians = glm::radians(static_cast<float>(kMaxFovDegrees));

    if (g_use_degrees) {
      int fov_degrees = static_cast<int>(glm::degrees(camera.fov));

      const char* fov_label = "%d degrees";
      if (fov_degrees == 110) {
        fov_label = "Quake Pro";
      }

      ImGui::SliderInt("FOV", &fov_degrees, kMinFovDegrees, kMaxFovDegrees, fov_label);
      camera.fov = glm::radians(static_cast<float>(fov_degrees));
    }
    else {
      ImGui::SliderFloat("FOV", &camera.fov, kMinFovRadians, kMaxFovRadians, "%.2f radians");
    }
  }

  ImGui::End();
}

}  // namespace Skeleton
