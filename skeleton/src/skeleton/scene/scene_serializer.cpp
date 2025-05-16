// Copyright 2024-2025 SkeletonEngine

#include "skeleton/scene/scene_serializer.hpp"
#include "skeleton/core/core.hpp"

#include <fstream>
#include <string>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include "skeleton/scene/components/components.hpp"
#include "skeleton/scene/entity.hpp"

namespace Skeleton {

static void DeserializeNode(entt::registry& scene, const nlohmann::json& json, entt::entity parent) {
  std::string name = json["name"];
  entt::entity entity = CreateEmptyEntity(&scene, parent, name);

  if (json.contains("camera")) {
    auto& camera = json["camera"];
    CameraComponent camera_component;
    camera_component.fov = camera["fov"];
    camera_component.clip_near = camera["clip_near"];
    camera_component.clip_far = camera["clip_far"];
    scene.emplace<CameraComponent>(entity, camera_component);
  }

  if (json.contains("clear_color")) {
    float r = json["clear_color"]["r"];
    float g = json["clear_color"]["g"];
    float b = json["clear_color"]["b"];
    float a = json["clear_color"]["a"];
    scene.emplace<ClearColorComponent>(entity, Color(r, g, b, a));
  }

  if (json.contains("children")) {
    for (const auto& child : json["children"]) {
      DeserializeNode(scene, child, entity);
    }
  }
}

entt::registry LoadScene(const std::string& path) {
  // Load file into a json object for parsing
  std::ifstream file(path);
  SK_ASSERT(file.is_open());
  nlohmann::json json;
  file >> json;
  file.close();

  // Recursively deserialize the scene
  entt::registry scene;
  DeserializeNode(scene, json, entt::null);
  return scene;
}

void SerializeNode(entt::registry& scene, const nlohmann::json& json) {
}

void SaveScene(const std::string& path, entt::registry* scene) {
  nlohmann::json json;
  SerializeNode(*scene, json);

  std::ofstream file(path);
  file << json.dump(2);
  file.close();
}

}  // namespace Skeleton
