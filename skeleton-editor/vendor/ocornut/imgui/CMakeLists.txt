cmake_minimum_required(VERSION 3.21)

set(IMGUI_CORE_FILES
  imgui/imgui.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_widgets.cpp
  imgui/imgui_tables.cpp
  imgui/imgui_demo.cpp
  imgui/imgui_internal.h
  imgui/imconfig.h
  imgui/imgui.h
  imgui/imstb_rectpack.h
  imgui/imstb_textedit.h
  imgui/imstb_truetype.h
)

set(IMGUI_GLFW_FILES
  imgui/backends/imgui_impl_glfw.cpp
  imgui/backends/imgui_impl_glfw.h
)

set(IMGUI_OPENGL_FILES
  imgui/backends/imgui_impl_opengl3.cpp
  imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_VULKAN_FILES
  imgui/backends/imgui_impl_vulkan.cpp
  imgui/backends/imgui_impl_vulkan.h
)

add_library(imgui
  ${IMGUI_CORE_FILES}
  ${IMGUI_GLFW_FILES}
  ${IMGUI_OPENGL_FILES}
  ${IMGUI_VULKAN_FILES}
)
target_compile_features(imgui PRIVATE cxx_std_20)

target_link_libraries(imgui PRIVATE glfw)
target_link_libraries(imgui PRIVATE volk)

target_include_directories(imgui PUBLIC imgui)
