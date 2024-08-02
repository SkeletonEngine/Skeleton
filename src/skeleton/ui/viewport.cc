#include "viewport.h"

#include <cstdint>
#include <memory>
#include <imgui/imgui.h>

namespace Skeleton {
namespace Ui {

static std::unique_ptr<Framebuffer> viewport_framebuffer;

void InitViewport() {
  viewport_framebuffer = std::make_unique<Framebuffer>(1280, 720);
}

void DrawViewport() {
  ImGui::Begin("Viewport");
  ImGui::Image((ImTextureID)(uint64_t)viewport_framebuffer->GetColorBufferHandle(), ImGui::GetContentRegionAvail());
  ImGui::End();
}

Framebuffer& GetViewportFramebuffer() {
  return *viewport_framebuffer;
}

}
}
