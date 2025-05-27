// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/gui/entity_properties_panel.hpp"
#include "skeleton/core/core.hpp"

#include <string>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "skeleton/scene/components/components.hpp"
#include "skeleton-editor/common/editor_preferences.hpp"
#include "skeleton-editor/gui/scene_graph_panel.hpp"
#include "skeleton-editor/gui/widgets/xyz_control.hpp"

namespace Skeleton {

void DrawEntityPropertiesPanel(entt::registry* scene, entt::entity root) {
  entt::entity entity = GetSelectedEntity();

  ImGui::Begin("Entity Properties");
  if (entity == entt::null) {
    ImGui::End();
    return;
  }

  // Display NameComponent
  auto& name = scene->get<NameComponent>(entity).name;
  ImGui::InputText("##Name", &name);

  // Display UuidComponent
  auto& uuid = scene->get<UuidComponent>(entity).uuid;
  ImGui::TextDisabled("%s", uuids::to_string(uuid).c_str());
  ImGui::Separator();

  // Display ClearColorComponent if it exists
  if (scene->any_of<ClearColorComponent>(entity)) {
    ImGui::Text("Clear Color");
    auto& clear_color = scene->get<ClearColorComponent>(entity).color;
    ImGui::ColorEdit4("##Clear Color", clear_color.rgba);
    ImGui::Separator();
  }

  // Display CurrentCameraComponent if it exists
  if (scene->any_of<CurrentCameraComponent>(entity)) {
    ImGui::Text("Current Camera");

    entt::entity current_camera = scene->get<CurrentCameraComponent>(entity).current_camera;

    // Display uuid of the current camera
    auto& uuid = scene->get<UuidComponent>(current_camera).uuid;
    ImGui::TextDisabled("%s", uuids::to_string(uuid).c_str());

    // Display name of the current camera
    auto& name = scene->get<NameComponent>(current_camera).name;
    ImGui::SameLine();
    ImGui::Text("(%s)", name.c_str());

    ImGui::Separator();
  }

  // Display CameraComponent if it exists
  if (scene->any_of<CameraComponent>(entity)) {
    ImGui::Text("Camera Settings");
    auto& camera = scene->get<CameraComponent>(entity);

    // Display FOV in either degrees or radians
    constexpr int kMinFovDegrees = 30;
    constexpr int kMaxFovDegrees = 150;
    constexpr float kMinFovRadians = glm::radians(static_cast<float>(kMinFovDegrees));
    constexpr float kMaxFovRadians = glm::radians(static_cast<float>(kMaxFovDegrees));

    if (g_use_degrees) {
      int fov_degrees = static_cast<int>(glm::degrees(camera.fov));

      const char* fov_label = "%d degrees";
      if (fov_degrees == 110) {
        fov_label = "Quake Pro";
      }

      if (ImGui::SliderInt("FOV", &fov_degrees, kMinFovDegrees, kMaxFovDegrees, fov_label)) {
        camera.projection_matrix_dirty = true;
      }
      camera.fov = glm::radians(static_cast<float>(fov_degrees));
    } else {
      if (ImGui::SliderFloat("FOV", &camera.fov, kMinFovRadians, kMaxFovRadians, "%.2f radians")) {
        camera.projection_matrix_dirty = true;
      }
    }

    if (ImGui::InputFloat("Near Clipping Plane", &camera.clip_near)) {
      camera.projection_matrix_dirty = true;
    }
    if (ImGui::InputFloat("Far Clipping Plane", &camera.clip_far)) {
      camera.projection_matrix_dirty = true;
    }

    bool is_current = scene->get<CurrentCameraComponent>(root).current_camera == entity;
    if (is_current) {
      ImGui::Text("This Camera is Current");
    } else {
      if (ImGui::Button("Make This Camera Current")) {
        scene->get<CurrentCameraComponent>(root).current_camera = entity;
        camera.projection_matrix_dirty = true;
      }
    }

    ImGui::Separator();
  }

  // Dispay Translation, rotation, and scale components under a single section
  if (scene->any_of<TranslationComponent>(entity) || scene->any_of<RotationComponent>(entity)) {
    ImGui::Text("Transform");

    // Display TranslationComponent if it exists
    if (scene->any_of<TranslationComponent>(entity)) {
      if (DrawXyzControl("Position", scene->get<TranslationComponent>(entity).translation)) {
        scene->get<TranslationComponent>(entity).translation_matrix_dirty = true;
      }
    }

    // Display RotationComponent if it exists
    if (scene->any_of<RotationComponent>(entity)) {
      if (DrawXyzControl("Rotation", scene->get<RotationComponent>(entity).rotation)) {
        scene->get<RotationComponent>(entity).translation_matrix_dirty = true;
      }
    }

    ImGui::Separator();
  }

  ImGui::End();
}

}  // namespace Skeleton
