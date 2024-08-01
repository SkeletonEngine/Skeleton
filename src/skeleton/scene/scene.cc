#include "scene.h"

namespace Skeleton {

Scene gScene;

Entity Scene::CreateEntity() {
  Entity e(registry);
  return e;
}

}
