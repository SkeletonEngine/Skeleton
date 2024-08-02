#pragma once

#include <imgui/imgui.h>
#include "renderer/framebuffer.h"

namespace Skeleton {
namespace Ui {

void InitDockspace();
void Draw(const Framebuffer& framebuffer);

}
}
