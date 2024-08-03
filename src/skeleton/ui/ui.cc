#include "ui.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "docs.h"
#include "panel_info.h"
#include "scene_graph.h"
#include "viewport.h"

namespace Skeleton {
namespace Ui {

static void InitDockspace() {
  ImGuiID dockspace = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

  ImGuiViewport* viewport = ImGui::GetMainViewport();
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
  ImGui::DockBuilderRemoveNode(dockspace); // clear any previous layout
  ImGui::DockBuilderAddNode(dockspace, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
  ImGui::DockBuilderSetNodeSize(dockspace, viewport->Size);

  ImGuiID dock_right = ImGui::DockBuilderSplitNode(dockspace, ImGuiDir_Right, 0.25f, nullptr, &dockspace);

  ImGui::DockBuilderDockWindow("Viewport", dockspace);
  ImGui::DockBuilderDockWindow("Scene Graph", dock_right);
  ImGui::DockBuilderDockWindow("Docs", dockspace);
}

void Init() {
  InitDockspace();
  InitViewport();
  LoadDocs();
  
  ShowPerspective(Perspective::kEditor);
}

void Draw() {
  ImGui::BeginMainMenuBar();
  if (ImGui::BeginMenu("View")) {
    if (ImGui::BeginMenu("Perspective")) {
      if (ImGui::MenuItem("Editor")) {
        ShowPerspective(Perspective::kEditor);
      }
      if (ImGui::MenuItem("Docs")) {
        ShowPerspective(Perspective::kDocs);
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Window")) {
      if (ImGui::MenuItem("Viewport", nullptr, gPanelsOpen[Panel::kViewport])) {
        gPanelsOpen[Panel::kViewport] = !gPanelsOpen[Panel::kViewport];
      }
      if (ImGui::MenuItem("Scene Graph", nullptr, gPanelsOpen[Panel::kSceneGraph])) {
        gPanelsOpen[Panel::kSceneGraph] = !gPanelsOpen[Panel::kSceneGraph];
      }
      if (ImGui::MenuItem("Docs", nullptr, gPanelsOpen[Panel::kDocs])) {
        gPanelsOpen[Panel::kDocs] = !gPanelsOpen[Panel::kDocs];
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenu();
  }
  ImGui::EndMainMenuBar();
  
  ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

  DrawViewport();
  DrawSceneGraph();
  DrawDocs();
}

}
}
