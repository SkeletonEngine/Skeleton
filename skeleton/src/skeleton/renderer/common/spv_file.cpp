// Copyright 2024 SkeletonEngine

#include "skeleton/renderer/common/spv_file.hpp"
#include "skeleton/core/core.hpp"

#include <fstream>

namespace Skeleton {

std::vector<char> ReadSpvFile(const char* path) {
  std::ifstream file(path, std::ios::ate | std::ios::binary);
  SK_ASSERT(file.is_open());
  size_t file_size = (size_t) file.tellg();
  std::vector<char> buffer(file_size);
  file.seekg(0);
  file.read(buffer.data(), file_size);
  file.close();
  return buffer;
}

}  // namespace Skeleton
