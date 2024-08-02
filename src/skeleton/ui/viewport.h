#pragma once

#include "renderer/framebuffer.h"

namespace Skeleton {
namespace Ui {

void InitViewport();
void DrawViewport();
Framebuffer& GetViewportFramebuffer();

}
}
