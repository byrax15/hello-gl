# CMakeList.txt : CMake project for hello-gl, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.24)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("hello-gl")

# libs
include(FetchContent)
find_package(gl3w CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glbinding CONFIG REQUIRED)
#find_package(glbinding)
#find_package(raylib CONFIG REQUIRED)
#FetchContent_Declare(
#	raylib_cpp
#	GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
#)
#FetchContent_MakeAvailable(raylib_cpp)
add_subdirectory(redbook_lib)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

# Add source to this project's executable.
add_executable (hello-gl "hello-gl.cpp")
target_link_libraries(hello-gl PRIVATE redbook_lib unofficial::gl3w::gl3w glfw glbinding::glbinding)


# copy shaders
add_custom_command(
	TARGET hello-gl	COMMAND ${CMAKE_COMMAND} -E copy_directory 
	"${CMAKE_SOURCE_DIR}/shaders" "$<TARGET_FILE_DIR:hello-gl>"
)