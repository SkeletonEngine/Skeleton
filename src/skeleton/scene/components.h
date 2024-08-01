#pragma once

#include <string>

#include "renderer/mesh.h"

namespace Skeleton {

struct NameComponent {
  std::string name;
  
  NameComponent() = default;
  NameComponent(const NameComponent&) = default;
  NameComponent(const std::string& name) : name(name) { }
};

struct MeshComponent {
  Mesh mesh;
  
  MeshComponent(const MeshComponent&) = default;
  MeshComponent(const Mesh& mesh) : mesh(mesh) { }
};

}
