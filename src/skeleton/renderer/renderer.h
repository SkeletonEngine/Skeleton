#pragma once

struct GLFWwindow;

namespace Skeleton {
namespace Renderer {

void Init(GLFWwindow* window);
void Shutdown();

void RenderScene();

}
}
