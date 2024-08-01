#pragma once

#include <entt/entity/registry.hpp>

#include "entity.h"
#include "components.h"

namespace Skeleton {

class Scene {
public:
  Entity CreateEntity();

public:
  entt::registry& Registry() { return registry; };

private:
  entt::registry registry;
};

extern Scene gScene;

}
