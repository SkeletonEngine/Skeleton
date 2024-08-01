#pragma once

#include <string>

namespace Skeleton {

struct NameComponent {
  std::string name;
  
  NameComponent() = default;
  NameComponent(const NameComponent&) = default;
  NameComponent(const std::string& name) : name(name) { }
};

}
