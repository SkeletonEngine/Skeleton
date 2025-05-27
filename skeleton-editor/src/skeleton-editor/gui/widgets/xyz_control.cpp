// Copyright 2024-2025 SkeletonEngine

#include "skeleton-editor/gui/widgets/xyz_control.hpp"
#include "skeleton/core/core.hpp"

#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Skeleton {

void DrawXyzControl(const char* label, glm::vec3& values, float reset_value) {
  ImGuiIO& io = ImGui::GetIO();
  auto bold_font = io.Fonts->Fonts[0];

  ImGui::PushID(label);

  ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

  float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
  ImVec2 button_size = { line_height + 3.0f, line_height };

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
  ImGui::PushFont(bold_font);
  if (ImGui::Button("X", button_size)) {
    values.x = reset_value;
  }
  ImGui::PopFont();
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
  ImGui::PopItemWidth();
  ImGui::SameLine();

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
  ImGui::PushFont(bold_font);
  if (ImGui::Button("Y", button_size)) {
    values.y = reset_value;
  }
  ImGui::PopFont();
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
  ImGui::PopItemWidth();
  ImGui::SameLine();

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
  ImGui::PushFont(bold_font);
  if (ImGui::Button("Z", button_size)) {
    values.z = reset_value;
  }
  ImGui::PopFont();
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
  ImGui::PopItemWidth();

  ImGui::PopStyleVar();

  ImGui::PopID();

  ImGui::SameLine();
  ImGui::Text("%s", label);
}

}  // namespace Skeleton
