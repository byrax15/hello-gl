cmake_minimum_required(VERSION 3.24)

project(redbook_lib)

find_package(gl3w CONFIG REQUIRED)

add_library(redbook_lib)
target_link_libraries(redbook_lib PRIVATE unofficial::gl3w::gl3w)

target_sources(redbook_lib PRIVATE src/LoadShaders.cpp)
target_sources(redbook_lib INTERFACE include/LoadShaders.h include/vgl.h)
target_include_directories(redbook_lib PRIVATE include)

set_target_properties(redbook_lib PROPERTIES LINKER_LANGUAGE CXX)