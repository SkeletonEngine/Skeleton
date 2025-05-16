cmake_minimum_required(VERSION 3.21)
project(skeleton)

# Run command to generate version header file when running cmake to generate project files
# This ensures that the version.hpp that will be generated can be found when including the list of files
include(cmake/skeleton_version_header_command.cmake)

include(cmake/skeleton_files.cmake)
include(cmake/skeleton_renderer_files.cmake)
include(cmake/skeleton_renderer_opengl_files.cmake)
include(cmake/skeleton_renderer_vulkan_files.cmake)

add_library(skeleton ${SKELETON_FILES})
target_compile_features(skeleton PRIVATE cxx_std_20)

target_include_directories(skeleton PUBLIC include)
target_include_directories(skeleton PRIVATE src)

# Choose targets to build for SPIRV-Cross
set(SPIRV_CROSS_ENABLE_GLSL TRUE CACHE BOOL "" FORCE)
set(SPIRV_CROSS_ENABLE_CPP FALSE CACHE BOOL "" FORCE)
set(SPIRV_CROSS_ENABLE_C_API FALSE CACHE BOOL "" FORCE)
set(SPIRV_CROSS_ENABLE_REFLECT FALSE CACHE BOOL "" FORCE)
set(SPIRV_CROSS_ENABLE_MSL FALSE CACHE BOOL "" FORCE)
set(SPIRV_CROSS_ENABLE_HLSL FALSE CACHE BOOL "" FORCE)
set(SPIRV_CROSS_ENABLE_UTIL FALSE CACHE BOOL "" FORCE)
set(SPIRV_CROSS_CLI FALSE CACHE BOOL "" FORCE)
add_subdirectory(vendor/khronosgroup/spirv-cross)
target_link_libraries(skeleton PRIVATE spirv-cross-glsl)

# Common Dependencies
add_subdirectory(vendor/glfw/glfw)
target_link_libraries(skeleton PRIVATE glfw)
add_subdirectory(vendor/g-truc/glm)
target_link_libraries(skeleton PUBLIC glm)
add_subdirectory(vendor/mariusbancilla/stduuid)
target_link_libraries(skeleton PUBLIC stduuid)
add_subdirectory(vendor/skypjack/entt)
target_link_libraries(skeleton PUBLIC EnTT)

# OpenGl Dependencies
add_subdirectory(vendor/dav1dde/glad)
target_link_libraries(skeleton PRIVATE glad)

# Vulkan Dependencies
add_subdirectory(vendor/zeux/volk)
target_link_libraries(skeleton PUBLIC volk)
add_subdirectory(vendor/gpuopen/vulkan-memory-allocator)
target_link_libraries(skeleton PUBLIC vulkan-memory-allocator)

include(cmake/skeleton_shader_compile_target.cmake)
add_dependencies(skeleton skeleton-shaders)

include(cmake/skeleton_version_header_target.cmake)
add_dependencies(skeleton skeleton-version-header)
