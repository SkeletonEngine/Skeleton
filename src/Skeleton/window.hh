#pragma once

struct GLFWwindow;

namespace Skeleton {
namespace Window {

void Create();
void Destroy();

bool IsOpen();
GLFWwindow* Get();

}
}
