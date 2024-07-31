#pragma once

#include "renderer_backend.h"

struct GLFWwindow;

namespace Skeleton {
namespace Renderer {

void Init(Backend backend);
void Shutdown();

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
