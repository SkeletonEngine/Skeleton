#include <imgui/imgui.h>

#include "renderer/framebuffer.h"
#include "renderer/mesh.h"
#include "renderer/renderer.h"
#include "scene/entity.h"
#include "scene/scene.h"
#include "ui/ui.h"
#include "ui/viewport.h"
#include "window.h"

int main() {
  Skeleton::Window window;
  Skeleton::Renderer::Internal::InitImGui();

  Skeleton::Renderer::BeginImGui();
  Skeleton::Ui::Init();
  Skeleton::Renderer::EndImGui();

  entt::entity e = Skeleton::gScene.CreateEntity();
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
  Skeleton::Mesh mesh(vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned short));
  Skeleton::Entity::AddComponent<Skeleton::MeshComponent>(e, mesh);

  while (window.IsOpen()) {
    window.PollEvents();
    
    Skeleton::Renderer::BeginFrame();
    Skeleton::Renderer::RenderScene(Skeleton::Ui::GetViewportFramebuffer());
    Skeleton::Renderer::BeginImGui();
    Skeleton::Ui::Draw();
    // ImGui::ShowDemoWindow();
    Skeleton::Renderer::EndImGui();
    Skeleton::Renderer::EndFrame();
  }
  
  Skeleton::Renderer::Internal::ShutdownImGui();
}
