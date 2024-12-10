// Copyright 2024 SkeletonEngine

#include "skeleton/window/window.hpp"
#include "skeleton/core/core.hpp"

#include <GLFW/glfw3.h>

namespace Skeleton {

Window::Window(const ApplicationSettings& settings) {
  SK_CHECK(glfwInit());

  switch (settings.renderer.backend) {
    case RendererBackend::kOpenGl: {
      glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
      break;
    }

    case RendererBackend::kVulkan: {
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      break;
    }
  }

  glfw_window = glfwCreateWindow(1280, 720, "Skeleton", NULL, NULL);
  SK_ASSERT(glfw_window);
}

Window::~Window() {
  glfwDestroyWindow(glfw_window);
  glfwTerminate();
}

void Window::PollEvents() const {
  glfwPollEvents();
}

bool Window::IsOpen() const {
  return !glfwWindowShouldClose(glfw_window);
}

GLFWwindow* Window::GetGlfwWindowHandle() const {
  return glfw_window;
}

int Window::GetFramebufferWidth() const {
  int width, height;
  glfwGetFramebufferSize(glfw_window, &width, &height);
  return width;
}

int Window::GetFramebufferHeight() const {
  int width, height;
  glfwGetFramebufferSize(glfw_window, &width, &height);
  return height;
}

}  // namespace Skeleton
