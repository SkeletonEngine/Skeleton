#include "properties.h"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include "panel_info.h"

namespace Skeleton {
namespace Ui {

static entt::entity e = entt::null;
static std::string name_buf;

void SetPropertiesPanelFocus(entt::entity entity) {
  e = entity;
  name_buf = Entity::GetComponent<NameComponent>(e).name;
}

void DrawPropertiesPanel() {
  if (!gPanelsOpen[Panel::kProperties]) return;

  ImGui::Begin("Properties", &gPanelsOpen[Panel::kProperties]);
  if (e != entt::null) {
    if (ImGui::InputText("Name", &name_buf)) {
      Entity::GetComponent<NameComponent>(e).name = name_buf;
    }

    if (Entity::HasComponent<MeshComponent>(e)) {
      if (ImGui::CollapsingHeader("Mesh")) {

      }
    }
  }
  ImGui::End();
}

}
}
