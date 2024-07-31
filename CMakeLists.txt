cmake_minimum_required(VERSION 3.10)
project(Skeleton)

set(SKELETON_SRC_FILES
  src/skeleton/main.cc
  src/skeleton/window.h
  src/skeleton/window.cc
)
source_group("src/skeleton" FILES ${SKELETON_SRC_FILES})

add_executable(Skeleton ${SKELETON_SRC_FILES})

add_subdirectory(src/lib/glfw)
target_link_libraries(Skeleton PRIVATE glfw)
