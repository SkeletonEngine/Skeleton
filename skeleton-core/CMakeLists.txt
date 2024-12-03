cmake_minimum_required(VERSION 3.10)
project(skeleton-core)

include(cmake/core_files.cmake)
include(cmake/core_renderer_files.cmake)
include(cmake/opengl_renderer_files.cmake)
include(cmake/vulkan_renderer_files.cmake)

add_library(skeleton-core ${SKELETON_CORE_FILES})
target_compile_features(skeleton-core PRIVATE cxx_std_17)

target_include_directories(skeleton-core PUBLIC include)
target_include_directories(skeleton-core PRIVATE src)

add_subdirectory(vendor/glfw/glfw)
target_link_libraries(skeleton-core PRIVATE glfw)
