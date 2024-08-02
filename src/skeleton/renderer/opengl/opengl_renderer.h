#pragma once

#include "renderer/framebuffer.h"

struct GLFWwindow;

namespace Skeleton {
namespace Renderer {

void BeginFrame();
void EndFrame();

void RenderScene(const Framebuffer& framebuffer);
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
