#include <imgui/imgui.h>

#include "window.h"
#include "renderer/renderer.h"

int main() {
  Skeleton::Window window;
  Skeleton::Renderer::Internal::InitImGui();

  while (window.IsOpen()) {
    window.PollEvents();
    
    Skeleton::Renderer::BeginFrame();
    Skeleton::Renderer::BeginImGui();
    ImGui::ShowDemoWindow();
    Skeleton::Renderer::RenderScene();
    Skeleton::Renderer::EndImGui();
    Skeleton::Renderer::EndFrame();
  }
  
  Skeleton::Renderer::Internal::ShutdownImGui();
}
