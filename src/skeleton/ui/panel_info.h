#pragma once

namespace Skeleton {
namespace Ui {

enum Panel {
  kViewport,
  kSceneGraph,
  kDocs,
  PANEL_ENUM_SIZE,
};

enum class Perspective {
  kEditor,
  kDocs,
};

extern bool gPanelsOpen[];

void ShowPerspective(Perspective perspective);

}
}
