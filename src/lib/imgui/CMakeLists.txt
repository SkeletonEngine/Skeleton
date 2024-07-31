cmake_minimum_required(VERSION 3.10)
project(imgui)

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
)

target_compile_features(imgui PRIVATE cxx_std_17)

target_include_directories(imgui PUBLIC ./..)
