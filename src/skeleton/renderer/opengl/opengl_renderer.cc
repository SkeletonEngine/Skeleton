#include "opengl_renderer.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "scene/scene.h"
#include "opengl_mesh.h"
#include "opengl_entity_shader.h"
#include "opengl_framebuffer.h"

namespace Skeleton {
namespace Renderer {

static GLFWwindow* window;
static EntityShader* entity_shader;

void BeginFrame() {
}

void EndFrame() {
  glfwSwapBuffers(window);
}

void RenderScene(const Framebuffer& framebuffer) {
  framebuffer.Bind();
  RenderScene();
}

void RenderScene() {
  glClear(GL_COLOR_BUFFER_BIT);
  
  entity_shader->Bind();

  auto view = gScene.Registry().view<MeshComponent>();
  for (auto& e : view) {
    auto& mesh = view.get<MeshComponent>(e);
    mesh.mesh.Draw();
  }
  
  entity_shader->Unbind();
}

void BeginImGui() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void EndImGui() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}
}

namespace Skeleton {
namespace Renderer {
namespace Internal {

void ApplyGlfwWindowHints() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
}

void CreateContext(GLFWwindow* glfw_window) {
  window = glfw_window;
  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);
  
  glfwSwapInterval(1);
  glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
  
  entity_shader = new EntityShader();
}

void InitImGui() {
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
  io.IniFilename = nullptr;
}

void ShutdownImGui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

}
}
}
