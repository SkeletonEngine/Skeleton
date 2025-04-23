// Copyright 2024-2025 SkeletonEngine

#include "skeleton/window/glfw/glfw_window.hpp"
#include "skeleton/core/core.hpp"

#include <string>
#include <GLFW/glfw3.h>

namespace Skeleton {

GlfwWindow::GlfwWindow(const WindowSettings& settings) {
  SK_CHECK(glfwInit());

  switch (settings.renderer_backend) {
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

  if (settings.open_maximized) {
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  }

  glfw_window_ = glfwCreateWindow(1280, 720, "Skeleton", NULL, NULL);
  SK_ASSERT(glfw_window_);

  /* Register callbacks */
  glfwSetWindowUserPointer(glfw_window_, this);
  glfwSetFramebufferSizeCallback(glfw_window_, [](GLFWwindow* w, int width, int height) {
    GlfwWindow* window = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(w));
    if (window->framebuffer_size_callback_) {
      window->framebuffer_size_callback_(width, height);
    }
  });
}

GlfwWindow::~GlfwWindow() {
  glfwDestroyWindow(glfw_window_);
  glfwTerminate();
}

void GlfwWindow::PollEvents() const {
  glfwPollEvents();
}

bool GlfwWindow::IsOpen() const {
  return !glfwWindowShouldClose(glfw_window_);
}

void GlfwWindow::RegisterFramebufferSizeCallback(std::function<void(int width, int height)> callback) {
  framebuffer_size_callback_ = callback;
}

GLFWwindow* GlfwWindow::GetNativeWindowHandle() const {
  return glfw_window_;
}

int GlfwWindow::GetFramebufferWidth() const {
  int width, height;
  glfwGetFramebufferSize(glfw_window_, &width, &height);
  return width;
}

int GlfwWindow::GetFramebufferHeight() const {
  int width, height;
  glfwGetFramebufferSize(glfw_window_, &width, &height);
  return height;
}

void GlfwWindow::SetTitle(const std::string& title) {
  glfwSetWindowTitle(glfw_window_, title.c_str());
}

}  // namespace Skeleton
