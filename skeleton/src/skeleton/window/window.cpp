// Copyright 2024-2025 SkeletonEngine

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

  glfw_window_ = glfwCreateWindow(1280, 720, "Skeleton", NULL, NULL);
  SK_ASSERT(glfw_window_);

  /* Register callbacks */
  glfwSetWindowUserPointer(glfw_window_, this);
  glfwSetFramebufferSizeCallback(glfw_window_, [](GLFWwindow* w, int width, int height) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(w));
    if (window->framebuffer_size_callback_) {
      window->framebuffer_size_callback_(width, height);
    }
  });
}

Window::~Window() {
  glfwDestroyWindow(glfw_window_);
  glfwTerminate();
}

void Window::PollEvents() const {
  glfwPollEvents();
}

bool Window::IsOpen() const {
  return !glfwWindowShouldClose(glfw_window_);
}

void Window::RegisterFramebufferSizeCallback(std::function<void(int width, int height)> callback) {
  framebuffer_size_callback_ = callback;
}

GLFWwindow* Window::GetGlfwWindowHandle() const {
  return glfw_window_;
}

int Window::GetFramebufferWidth() const {
  int width, height;
  glfwGetFramebufferSize(glfw_window_, &width, &height);
  return width;
}

int Window::GetFramebufferHeight() const {
  int width, height;
  glfwGetFramebufferSize(glfw_window_, &width, &height);
  return height;
}

}  // namespace Skeleton
