# CMakeList.txt : CMake project for hello-gl, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.19)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("hello-gl")

# libs
include(FetchContent)
find_package(glfw3 CONFIG REQUIRED)
find_package(glbinding CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
#find_package(raylib CONFIG REQUIRED)
#FetchContent_Declare(
#	raylib_cpp
#	GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
#)
#FetchContent_MakeAvailable(raylib_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

# Add source to this project's executable.
add_executable (hello-gl)
target_sources(hello-gl PRIVATE "hello-gl.cpp" "Shader.h" "GLstate.h")

target_link_libraries(hello-gl PRIVATE glbinding::glbinding glfw glm::glm)

if(MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT hello-gl)
endif()

# copy shaders
add_custom_command(
	TARGET hello-gl	COMMAND ${CMAKE_COMMAND} -E copy_directory 
	"${CMAKE_SOURCE_DIR}/shaders" "$<TARGET_FILE_DIR:hello-gl>"
)
