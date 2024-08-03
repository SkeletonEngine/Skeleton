#include "scene_graph.h"

#include <imgui/imgui.h>

#include "panel_info.h"
#include "properties.h"
#include "scene/scene.h"

namespace Skeleton {
namespace Ui {

void DrawSceneGraphPanel() {
  if (!gPanelsOpen[Panel::kSceneGraph]) return;
  
  ImGui::Begin("Scene Graph", &gPanelsOpen[Panel::kSceneGraph]);

  auto view = gScene.Registry().view<NameComponent>();
  for (auto& e : view) {
    auto& name = view.get<NameComponent>(e);
    if (ImGui::TreeNode(name.name.c_str())) {
      SetPropertiesPanelFocus(e);
      ImGui::TreePop();
    }
  }

  ImGui::End();
}

}
}
