cmake_minimum_required(VERSION 3.19)

project(gl_abstractions)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glbinding CONFIG REQUIRED)

add_library(gl_abstractions STATIC)

target_sources(gl_abstractions PRIVATE "src/gl_job.cpp")
target_sources(gl_abstractions PRIVATE "include/gl_abstractions.h")

target_include_directories(gl_abstractions PRIVATE "include" "../redbook_lib/include")
target_link_libraries(gl_abstractions PRIVATE glbinding::glbinding)

set_target_properties(gl_abstractions PROPERTIES LINKER_LANGUAGE CXX)