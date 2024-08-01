#include <imgui/imgui.h>

#include "renderer/renderer.h"
#include "ui/ui.h"
#include "window.h"
#include "scene/scene.h"

int main() {
  Skeleton::Window window;
  Skeleton::Renderer::Internal::InitImGui();

  Skeleton::Renderer::BeginImGui();
  Skeleton::Ui::InitDockspace();
  Skeleton::Renderer::EndImGui();

  Skeleton::Entity e = Skeleton::gScene.CreateEntity();
  float vertices[] = {
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
   -0.5f, -0.5f, 0.0f,  // bottom left
   -0.5f,  0.5f, 0.0f   // top left 
  };
  unsigned short indices[] = {
    0, 1, 3,  // first triangle
    1, 2, 3   // second triangle
  };
  Skeleton::Mesh mesh(vertices, 12, indices, 6);
  e.AddComponent<Skeleton::MeshComponent>(mesh);

  while (window.IsOpen()) {
    window.PollEvents();
    
    Skeleton::Renderer::BeginFrame();
    Skeleton::Renderer::RenderScene();
    Skeleton::Renderer::BeginImGui();
    // Skeleton::Ui::Draw();
    // ImGui::ShowDemoWindow();
    Skeleton::Renderer::EndImGui();
    Skeleton::Renderer::EndFrame();
  }
  
  Skeleton::Renderer::Internal::ShutdownImGui();
}
