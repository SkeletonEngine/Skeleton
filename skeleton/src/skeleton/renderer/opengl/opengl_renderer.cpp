// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/opengl/opengl_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <string>
#include <sstream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "skeleton/core/version.hpp"
#include "skeleton/window/glfw/glfw_window.hpp"

namespace Skeleton::OpenGl {

OpenGlRenderer::OpenGlRenderer(const RendererSettings& settings) {
  glfw_window_ = dynamic_cast<GlfwWindow*>(settings.window)->GetNativeWindowHandle();
  glfwMakeContextCurrent(glfw_window_);
  gladLoadGL(glfwGetProcAddress);
}

void OpenGlRenderer::RenderFrame() {
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(glfw_window_);
}

std::string OpenGlRenderer::GetRendererString() const {
  std::stringstream ss;
  ss << "Skeleton " << SK_VERSION_STRING;
  ss << " [Renderer: OpenGl " << glGetString(GL_VERSION) << "]";
  ss << " [GPU: " << glGetString(GL_RENDERER) << "]";

  return ss.str();
}

void OpenGlRenderer::SetScene(entt::registry* scene, entt::entity root) {
}

}  // namespace Skeleton::OpenGl
