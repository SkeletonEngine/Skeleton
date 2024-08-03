#pragma once

#include <entt/entity/registry.hpp>
#include "components.h"
#include "scene.h"

namespace Skeleton {
namespace Entity {

template<typename T, typename... Args>
T& AddComponent(entt::entity e, Args&&... args) {
	// assert(!HasComponent<T>(), "Entity already has component!");
	T& component = gScene.Registry().emplace<T>(e, std::forward<Args>(args)...);
	// scene->OnComponentAdded<T>(*this, component);
	return component;
}

template<typename T, typename... Args>
T& AddOrReplaceComponent(entt::entity e, Args&&... args) {
	T& component = gScene.Registry().emplace_or_replace<T>(e, std::forward<Args>(args)...);
	// scene->OnComponentAdded<T>(*this, component);
	return component;
}

template<typename T>
T& GetComponent(entt::entity e) {
	// assert(HasComponent<T>(), "Entity does not have component!");
	return gScene.Registry().get<T>(e);
}

template<typename T>
bool HasComponent(entt::entity e) {
	return gScene.Registry().all_of<T>(e);
}

template<typename T>
void RemoveComponent(entt::entity e) {
	// assert(HasComponent<T>(), "Entity does not have component!");
	gScene.Registry().remove<T>(e);
}

}
}
