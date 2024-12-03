#include "skeleton/window/window.hpp"

#include <GLFW/glfw3.h>

namespace Skeleton {

Window::Window(const ApplicationSettings& settings) {
  glfwInit();
  glfw_window = glfwCreateWindow(1280, 720, "Skeleton", NULL, NULL);
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

}
