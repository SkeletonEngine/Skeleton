cmake_minimum_required(VERSION 3.10)
project(Skeleton)

set(SKELETON_SRC_FILES
  src/skeleton/main.cc
  src/skeleton/window.h
  src/skeleton/window.cc
)
source_group("src/skeleton" FILES ${SKELETON_SRC_FILES})

set(SKELETON_SRC_RENDERER_FILES
  src/skeleton/renderer/renderer.h
  src/skeleton/renderer/renderer.cc
  src/skeleton/renderer/renderer_backend.h
)
source_group("src/skeleton/renderer" FILES ${SKELETON_SRC_RENDERER_FILES})

add_executable(Skeleton
  ${SKELETON_SRC_FILES}
  ${SKELETON_SRC_RENDERER_FILES}
)

add_subdirectory(src/lib/glfw)
target_link_libraries(Skeleton PRIVATE glfw)

add_subdirectory(src/lib/glad)
target_link_libraries(Skeleton PRIVATE glad)
