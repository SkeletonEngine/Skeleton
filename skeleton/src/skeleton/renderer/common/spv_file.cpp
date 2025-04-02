// Copyright 2024-2025 SkeletonEngine

#include "skeleton/renderer/common/spv_file.hpp"
#include "skeleton/core/core.hpp"

#include <cstdio>
#include <fstream>
#include <vector>

namespace Skeleton {

std::vector<uint32_t> ReadSpvFile(const char* path) {
  std::ifstream file(path, std::ios::ate | std::ios::binary);
  if (!file.is_open()) {
    fprintf(stderr, "Failed to open SPIR-V file: %s\n", path);
    SK_DEBUGBREAK();
  }
  size_t file_size = static_cast<size_t>(file.tellg());
  std::vector<uint32_t> buffer(file_size / sizeof(uint32_t));
  file.seekg(0);
  file.read(reinterpret_cast<char*>(buffer.data()), file_size);
  file.close();
  return buffer;
}

}  // namespace Skeleton
