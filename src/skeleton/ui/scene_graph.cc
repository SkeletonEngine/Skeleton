#include "scene_graph.h"

#include <imgui/imgui.h>

#include "scene/scene.h"

namespace Skeleton {
namespace Ui {

bool gSceneGraphWindowOpen = true;

void DrawSceneGraph() {
  if (!gSceneGraphWindowOpen) return;
  
  ImGui::Begin("Scene Graph", &gSceneGraphWindowOpen);

  auto view = gScene.Registry().view<NameComponent>();
  for (auto& e : view) {
    auto& name = view.get<NameComponent>(e);
    ImGui::Text("%s", name.name.c_str());
  }

  ImGui::End();
}

}
}
