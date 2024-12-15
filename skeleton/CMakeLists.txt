cmake_minimum_required(VERSION 3.21)
project(skeleton)

include(cmake/skeleton_files.cmake)
include(cmake/skeleton_renderer_files.cmake)
include(cmake/skeleton_renderer_opengl_files.cmake)
include(cmake/skeleton_renderer_vulkan_files.cmake)

add_library(skeleton ${SKELETON_FILES})
target_compile_features(skeleton PRIVATE cxx_std_20)

target_include_directories(skeleton PUBLIC include)
target_include_directories(skeleton PRIVATE src)

add_subdirectory(vendor/glfw/glfw)
target_link_libraries(skeleton PRIVATE glfw)
add_subdirectory(vendor/zeux/volk)
target_link_libraries(skeleton PRIVATE volk)

include(cmake/skeleton_shader_compile_target.cmake)
add_dependencies(skeleton skeleton-shaders)
