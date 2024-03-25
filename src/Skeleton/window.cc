#include "window.hh"

#include <GLFW/glfw3.h>

namespace Skeleton {
namespace Window {

static GLFWwindow* window;

void Create() {
  glfwInit();
  window = glfwCreateWindow(1280, 720, "Skeleton", nullptr, nullptr);
}

void Destroy() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

bool IsOpen() {
  return !glfwWindowShouldClose(window);
}

}
}
