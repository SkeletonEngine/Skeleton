cmake_minimum_required(VERSION 3.27.0)
project(Skeleton)

set(SKELETON_SRC_FILES
  src/Skeleton/main.cc
  src/Skeleton/input.hh  src/Skeleton/input.cc
  src/Skeleton/window.hh src/Skeleton/window.cc
)
source_group("src" FILES ${SKELETON_SRC_FILES})

add_executable(${PROJECT_NAME} ${SKELETON_SRC_FILES})

add_subdirectory(src/Extern/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
