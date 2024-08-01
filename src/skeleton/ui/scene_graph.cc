#include "scene_graph.h"

#include <imgui/imgui.h>

#include "scene/scene.h"

namespace Skeleton {
namespace Ui {

void DrawSceneGraph() {
  ImGui::Begin("Scene Graph");

  auto view = gScene.Registry().view<NameComponent>();
  for (auto& e : view) {
    auto& name = view.get<NameComponent>(e);
    ImGui::Text("%s", name.name.c_str());
  }

  ImGui::End();
}

}
}
