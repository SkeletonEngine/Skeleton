#include "panel_info.h"

#include <cstring>

namespace Skeleton {
namespace Ui {

bool gPanelsOpen[Panel::PANEL_ENUM_SIZE];

void ShowPerspective(Perspective perspective) {
  std::memset(gPanelsOpen, 0, Panel::PANEL_ENUM_SIZE);

  switch (perspective) {
  case Perspective::kEditor:
    gPanelsOpen[Panel::kViewport] = true;
    gPanelsOpen[Panel::kSceneGraph] = true;
    break;
  case Perspective::kDocs:
    gPanelsOpen[Panel::kDocs] = true;
    break;
  }
}

}
}
