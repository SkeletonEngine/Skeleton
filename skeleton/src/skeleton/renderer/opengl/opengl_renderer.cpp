// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/opengl/opengl_renderer.hpp"
#include "skeleton/core/core.hpp"

#include <string>
#include <sstream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "skeleton/window/glfw/glfw_window.hpp"

namespace Skeleton::OpenGl {

OpenGlRenderer::OpenGlRenderer(Window* window) {
  glfwMakeContextCurrent(dynamic_cast<GlfwWindow*>(window)->GetNativeWindowHandle());
  gladLoadGL(glfwGetProcAddress);
}

void OpenGlRenderer::RenderFrame() {
}

std::string OpenGlRenderer::GetRendererString() const {
  std::stringstream ss;
  ss << "Skeleton " << SK_VERSION_STRING;
  ss << " [Renderer: OpenGl " << glGetString(GL_VERSION) << "]";
  ss << " [GPU: " << glGetString(GL_RENDERER) << "]";

  return ss.str();
}

}  // namespace Skeleton::OpenGl
