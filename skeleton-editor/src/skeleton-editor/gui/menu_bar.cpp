// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/gui/menu_bar.hpp"
#include "skeleton/core/core.hpp"

#include <imgui/imgui.h>
#include "skeleton-editor/common/editor_preferences.hpp"

namespace Skeleton {

void DrawMenuBar() {
  ImGui::BeginMainMenuBar();

  if (ImGui::BeginMenu("File")) {
    ImGui::MenuItem("Open Scene");
    ImGui::MenuItem("Save Scene");
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Preferences")) {
    ImGui::MenuItem("Use Degrees", nullptr, &g_use_degrees);
    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();
}

}  // namespace Skeleton
