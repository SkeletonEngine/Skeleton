cmake_minimum_required(VERSION 3.27.0)
project(Skeleton)

add_executable(${PROJECT_NAME} Skeleton/src/main.cc)

if(PROJECT_IS_TOP_LEVEL)
  if(WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Skeleton)
  endif()
endif()
