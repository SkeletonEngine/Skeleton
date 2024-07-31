#include "renderer.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace Skeleton {
namespace Renderer {

void Init(GLFWwindow* window) {
  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);
}

void Shutdown() {

}

}
}
