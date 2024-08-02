cmake_minimum_required(VERSION 3.10)
project(imgui)

set(IMGUI_OPENGL_FILES
  imgui/backends/imgui_impl_opengl3.h
  imgui/backends/imgui_impl_opengl3.cpp
  imgui/backends/imgui_impl_glfw.h
  imgui/backends/imgui_impl_glfw.cpp
)

set(IMGUI_MISC_FILES
  imgui/misc/cpp/imgui_stdlib.h
  imgui/misc/cpp/imgui_stdlib.cpp
)

add_library(imgui
  imgui/imgui_demo.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_internal.h
  imgui/imgui_tables.cpp
  imgui/imgui_widgets.cpp
  imgui/imgui.cpp
  imgui/imgui.h
  imgui/imstb_rectpack.h
  imgui/imstb_textedit.h
  imgui/imstb_truetype.h

  ${IMGUI_OPENGL_FILES}
  ${IMGUI_MISC_FILES}
)

target_compile_features(imgui PRIVATE cxx_std_17)

target_include_directories(imgui PUBLIC .)
target_include_directories(imgui PUBLIC ./imgui)

target_link_libraries(imgui PRIVATE glfw)
