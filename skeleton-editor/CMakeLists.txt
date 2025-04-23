cmake_minimum_required(VERSION 3.21)
project(skeleton-editor)

set(SKELETON_EDITOR_SRC_FILES
  src/skeleton-editor/main.cpp
)
source_group("src/skeleton-editor" FILES ${SKELETON_EDITOR_SRC_FILES})

set(SKELETON_EDITOR_SRC_GUI_FILES
  src/skeleton-editor/gui/dockspace.hpp
  src/skeleton-editor/gui/dockspace.cpp
)
source_group("src/skeleton-editor/gui" FILES ${SKELETON_EDITOR_SRC_GUI_FILES})

set(SKELETON_EDITOR_RENDERER_OPENGL_FILES
  src/skeleton-editor/renderer/opengl/opengl_editor_renderer.hpp
  src/skeleton-editor/renderer/opengl/opengl_editor_renderer.cpp
)
source_group("src/skeleton-editor/renderer/opengl" FILES ${SKELETON_EDITOR_RENDERER_OPENGL_FILES})

set(SKELETON_EDITOR_RENDERER_VULKAN_FILES
  src/skeleton-editor/renderer/vulkan/vulkan_editor_renderer.hpp
  src/skeleton-editor/renderer/vulkan/vulkan_editor_renderer.cpp
  src/skeleton-editor/renderer/vulkan/vulkan_editor_renderer_descriptor_pool.cpp
  src/skeleton-editor/renderer/vulkan/vulkan_editor_renderer_imgui_render_pass.cpp
  src/skeleton-editor/renderer/vulkan/vulkan_editor_renderer_viewport_framebuffers.cpp
  src/skeleton-editor/renderer/vulkan/vulkan_editor_renderer_viewport.cpp
)
source_group("src/skeleton-editor/renderer/vulkan" FILES ${SKELETON_EDITOR_RENDERER_VULKAN_FILES})

add_executable(skeleton-editor
  ${SKELETON_EDITOR_SRC_FILES}
  ${SKELETON_EDITOR_SRC_GUI_FILES}
  ${SKELETON_EDITOR_RENDERER_OPENGL_FILES}
  ${SKELETON_EDITOR_RENDERER_VULKAN_FILES}
)
target_compile_features(skeleton-editor PRIVATE cxx_std_20)

target_include_directories(skeleton-editor PRIVATE src)

target_link_libraries(skeleton-editor PRIVATE skeleton)

add_subdirectory(vendor/ocornut/imgui)
target_link_libraries(skeleton-editor PRIVATE imgui)

set_target_properties(skeleton-editor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
set_target_properties(skeleton-editor PROPERTIES XCODE_GENERATE_SCHEME TRUE XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
