cmake_minimum_required(VERSION 3.10)
project(skeleton-editor)

set(SKELETON_EDITOR_SRC_FILES
  src/skeleton-editor/main.cpp
)
source_group("src/skeleton-editor" FILES ${SKELETON_EDITOR_SRC_FILES})

add_executable(skeleton-editor ${SKELETON_EDITOR_SRC_FILES})
target_compile_features(skeleton-editor PRIVATE cxx_std_17)

target_link_libraries(skeleton-editor PRIVATE skeleton-core)
