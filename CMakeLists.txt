cmake_minimum_required(VERSION 3.10)
project(Skeleton)

set(SKELETON_SRC_FILES
  src/skeleton/main.cc
  src/skeleton/window.h
  src/skeleton/window.cc
)
source_group("src/skeleton" FILES ${SKELETON_SRC_FILES})

set(SKELETON_SRC_UI_FILES
  src/skeleton/ui/ui.h
  src/skeleton/ui/ui.cc
  src/skeleton/ui/viewport.h
  src/skeleton/ui/viewport.cc
  src/skeleton/ui/scene_graph.h
  src/skeleton/ui/scene_graph.cc
  src/skeleton/ui/docs.h
  src/skeleton/ui/docs.cc
)
source_group("src/skeleton/ui" FILES ${SKELETON_SRC_UI_FILES})

set(SKELETON_SRC_RENDERER_FILES
  src/skeleton/renderer/renderer.h
  src/skeleton/renderer/mesh.h
  src/skeleton/renderer/framebuffer.h
)
source_group("src/skeleton/renderer" FILES ${SKELETON_SRC_RENDERER_FILES})

set(SKELETON_SRC_RENDERER_OPENGL_FILES
  src/skeleton/renderer/opengl/opengl_renderer.h
  src/skeleton/renderer/opengl/opengl_renderer.cc
  src/skeleton/renderer/opengl/opengl_mesh.h
  src/skeleton/renderer/opengl/opengl_mesh.cc
  src/skeleton/renderer/opengl/opengl_shader.h
  src/skeleton/renderer/opengl/opengl_shader.cc
  src/skeleton/renderer/opengl/opengl_entity_shader.h
  src/skeleton/renderer/opengl/opengl_entity_shader.cc
  src/skeleton/renderer/opengl/opengl_framebuffer.h
  src/skeleton/renderer/opengl/opengl_framebuffer.cc
)
source_group("src/skeleton/renderer/opengl" FILES ${SKELETON_SRC_RENDERER_OPENGL_FILES})

set(SKELETON_SRC_SCENE_FILES
  src/skeleton/scene/scene.h
  src/skeleton/scene/scene.cc
  src/skeleton/scene/entity.h
  src/skeleton/scene/entity.cc
  src/skeleton/scene/components.h
)
source_group("src/skeleton/scene" FILES ${SKELETON_SRC_SCENE_FILES})

add_executable(Skeleton
  ${SKELETON_SRC_FILES}
  ${SKELETON_SRC_UI_FILES}
  ${SKELETON_SRC_RENDERER_FILES}
  ${SKELETON_SRC_RENDERER_OPENGL_FILES}
  ${SKELETON_SRC_SCENE_FILES}
)

target_compile_features(Skeleton PRIVATE cxx_std_17)
target_include_directories(Skeleton PRIVATE src/skeleton)

add_subdirectory(src/lib/glfw)
target_link_libraries(Skeleton PRIVATE glfw)

add_subdirectory(src/lib/glad)
target_link_libraries(Skeleton PRIVATE glad)

add_subdirectory(src/lib/imgui)
target_link_libraries(Skeleton PRIVATE imgui)

add_subdirectory(src/lib/entt)
target_link_libraries(Skeleton PRIVATE EnTT)

if(APPLE)
  set_target_properties(Skeleton PROPERTIES  XCODE_GENERATE_SCHEME TRUE XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()
