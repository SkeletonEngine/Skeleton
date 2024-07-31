#include "window.h"

#include <GLFW/glfw3.h>
#include "renderer/renderer.h"

namespace Skeleton {

Window::Window() {
  glfwInit();
  Renderer::Internal::ApplyGlfwWindowHints();
  window = glfwCreateWindow(1280, 720, "Test", nullptr, nullptr);
  Renderer::Internal::CreateContext(window);
}

Window::~Window() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

bool Window::IsOpen() const {
  return !glfwWindowShouldClose(window);
}

void Window::PollEvents() {
  glfwPollEvents();
}

}
