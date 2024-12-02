cmake_minimum_required(VERSION 3.10)
project(skeleton-core)

set(SKELETON_CORE_SRC_FILES
  include/skeleton/skeleton.hpp
  src/skeleton/skeleton.cpp
)
source_group("src/skeleton-core" FILES ${SKELETON_CORE_SRC_FILES})

add_library(skeleton-core ${SKELETON_CORE_SRC_FILES})

target_include_directories(skeleton-core PUBLIC include)
target_include_directories(skeleton-core PRIVATE src/skeleton)
