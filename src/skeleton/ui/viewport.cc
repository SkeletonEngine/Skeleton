#include "viewport.h"

#include <cstdint>
#include <memory>
#include <imgui/imgui.h>

namespace Skeleton {
namespace Ui {

bool gViewportWindowOpen = true;

static std::unique_ptr<Framebuffer> framebuffer;

void InitViewport() {
  framebuffer = std::make_unique<Framebuffer>(1280, 720);
}

void DrawViewport() {
  if (!gViewportWindowOpen) return;
  
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
  ImGui::Begin("Viewport", &gViewportWindowOpen);
  static ImVec2 last_size = { 0, 0 };
  ImVec2 size = ImGui::GetContentRegionAvail();
  if (size.x != last_size.x || size.y != last_size.y) {
    framebuffer->Resize(size.x, size.y);
    last_size = size;
  }
  ImGui::Image((ImTextureID)(uint64_t)framebuffer->GetColorBufferHandle(), size);
  ImGui::End();
  ImGui::PopStyleVar();
}

Framebuffer& GetViewportFramebuffer() {
  return *framebuffer;
}

}
}
