#include "ui.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "scene_graph.h"
#include "viewport.h"

namespace Skeleton {
namespace Ui {

void InitDockspace() {
  ImGuiID dockspace = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
  ImGui::DockBuilderRemoveNode(dockspace); // clear any previous layout
	ImGui::DockBuilderAddNode(dockspace, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(dockspace, viewport->Size);

  ImGuiID dock_right = ImGui::DockBuilderSplitNode(dockspace, ImGuiDir_Right, 0.25f, nullptr, &dockspace);

  ImGui::DockBuilderDockWindow("Viewport", dockspace);
  ImGui::DockBuilderDockWindow("Scene Graph", dock_right);
}

void Draw() {
  ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
  DrawViewport();
  DrawSceneGraph();
}

}
}
