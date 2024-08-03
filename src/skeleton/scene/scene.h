#pragma once

#include <entt/entity/registry.hpp>
#include "components.h"

namespace Skeleton {

class Scene {
public:
  inline entt::entity CreateEntity() {
    entt::entity e = registry.create();
	  registry.emplace<NameComponent>(e, "Entity");
    return e;
  }

public:
  entt::registry& Registry() { return registry; };

private:
  entt::registry registry;
};

extern Scene gScene;

}
