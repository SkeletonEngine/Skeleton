#include "renderer.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace Skeleton {
namespace Renderer {

void Init(Backend backend) {
}

void Shutdown() {
}

}
}

namespace Skeleton {
namespace Renderer {
namespace Internal {

void ApplyGlfwWindowHints() {
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
}

void CreateContext(GLFWwindow* window) {
  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);
}

}
}
}
