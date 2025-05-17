// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/gui/menu_bar.hpp"
#include "skeleton/core/core.hpp"

#include <entt/entt.hpp>
#include <imgui/imgui.h>
#include "skeleton/scene/scene_serializer.hpp"
#include "skeleton-editor/common/editor_preferences.hpp"

namespace Skeleton {

void DrawMenuBar(entt::registry* scene) {
  ImGui::BeginMainMenuBar();

  if (ImGui::BeginMenu("File")) {
    if (ImGui::MenuItem("Open Scene")) {
      *scene = LoadScene("user-data/user-scene.skscene");
    }
    if (ImGui::MenuItem("Save Scene")) {
      SaveScene("user-data/user-scene.skscene", scene);
    }
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Preferences")) {
    ImGui::MenuItem("Use Degrees", nullptr, &g_use_degrees);
    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();
}

}  // namespace Skeleton
