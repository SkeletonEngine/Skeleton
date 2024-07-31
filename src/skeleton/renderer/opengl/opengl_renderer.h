#pragma once

struct GLFWwindow;

namespace Skeleton {
namespace Renderer {

void BeginFrame();
void EndFrame();

void RenderScene();

void BeginImGui();
void EndImGui();

}
}

namespace Skeleton {
namespace Renderer {
namespace Internal {

void ApplyGlfwWindowHints();
void CreateContext(GLFWwindow* window);

void InitImGui();
void ShutdownImGui();

}
}
}
