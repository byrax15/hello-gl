cmake_minimum_required(VERSION 3.19)

project(redbook_lib)

find_package(glbinding CONFIG REQUIRED)

add_library(redbook_lib STATIC)
target_sources(redbook_lib PRIVATE src/LoadShaders.cpp)
target_sources(redbook_lib PRIVATE include/LoadShaders.h include/vgl.h)
target_include_directories(redbook_lib PRIVATE include)
target_link_libraries(redbook_lib PRIVATE glbinding::glbinding)


set_target_properties(redbook_lib PROPERTIES LINKER_LANGUAGE CXX)