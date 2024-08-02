#include "viewport.h"

#include <imgui/imgui.h>

namespace Skeleton {
namespace Ui {

void DrawViewport(const Framebuffer& framebuffer) {
  ImGui::Begin("Viewport");
  ImGui::Image((ImTextureID)framebuffer.GetColorBufferHandle(), ImGui::GetContentRegionAvail());
  ImGui::End();
}

}
}
