#pragma once

#include "renderer/framebuffer.h"

namespace Skeleton {
namespace Ui {

extern bool gViewportWindowOpen;

void InitViewport();
void DrawViewport();
Framebuffer& GetViewportFramebuffer();

}
}
