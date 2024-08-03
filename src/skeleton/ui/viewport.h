#pragma once

#include "renderer/framebuffer.h"

namespace Skeleton {
namespace Ui {

void InitViewportPanel();
void DrawViewportPanel();
Framebuffer& GetViewportFramebuffer();

}
}
