cmake_minimum_required(VERSION 3.21)
project(skeleton-runtime)

set(SKELETON_RUNTIME_SRC_FILES
  src/skeleton-runtime/main.cpp
)
source_group("src/skeleton-runtime" FILES ${SKELETON_RUNTIME_SRC_FILES})

add_executable(skeleton-runtime ${SKELETON_RUNTIME_SRC_FILES})
target_compile_features(skeleton-runtime PRIVATE cxx_std_20)

target_link_libraries(skeleton-runtime PRIVATE skeleton)

set_target_properties(skeleton-runtime PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
set_target_properties(skeleton-runtime PROPERTIES XCODE_GENERATE_SCHEME TRUE XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
