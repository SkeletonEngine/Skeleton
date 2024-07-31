#pragma once

struct GLFWwindow;

namespace Skeleton {
namespace Renderer {

void RenderScene();

}
}

namespace Skeleton {
namespace Renderer {
namespace Internal {

void ApplyGlfwWindowHints();
void CreateContext(GLFWwindow* window);

}
}
}
