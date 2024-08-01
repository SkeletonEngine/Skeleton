#include <imgui/imgui.h>

#include "window.h"
#include "renderer/renderer.h"
#include "ui/ui.h"

int main() {
  Skeleton::Window window;
  Skeleton::Renderer::Internal::InitImGui();

  Skeleton::Renderer::BeginImGui();
  Skeleton::Ui::InitDockspace();
  Skeleton::Renderer::EndImGui();

  while (window.IsOpen()) {
    window.PollEvents();
    
    Skeleton::Renderer::BeginFrame();
    Skeleton::Renderer::BeginImGui();
    Skeleton::Ui::Draw();
    // ImGui::ShowDemoWindow();
    Skeleton::Renderer::RenderScene();
    Skeleton::Renderer::EndImGui();
    Skeleton::Renderer::EndFrame();
  }
  
  Skeleton::Renderer::Internal::ShutdownImGui();
}
