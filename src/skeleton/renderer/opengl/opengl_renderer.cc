#include "opengl_renderer.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace Skeleton {
namespace Renderer {

static GLFWwindow* window;

void BeginFrame() {
}

void EndFrame() {
  glfwSwapBuffers(window);
}

void RenderScene() {
  glClear(GL_COLOR_BUFFER_BIT);
}

void BeginImGui() {
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
#ifdef __APPLE__
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
}

void CreateContext(GLFWwindow* glfw_window) {
  window = glfw_window;
  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);
  
  glfwSwapInterval(1);
  glClearColor(0.2, 0.4, 0.6, 1.0);
}

void InitImGui() {
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");
}

void ShutdownImGui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

}
}
}
