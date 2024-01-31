cmake_minimum_required(VERSION 3.5.0)
project(HelloWorld VERSION 1.0.0)
cmake_policy(SET CMP0076 NEW)




find_package(Git QUIET)
if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
	option(CTEST_GIT_INIT_SUBMODULES "Initialize git submodules during build" ON)
	if(CTEST_GIT_INIT_SUBMODULES)
		message(STATUS "Initializing git submodules")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
endif()


set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# looking for opengl
message(STATUS "Looking for OpenGL")
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
  message(STATUS "OpenGL found")
  else()
  message(FATAL_ERROR "OpenGL not found")
endif(OPENGL_FOUND)


add_subdirectory(dep/glfw EXCLUDE_FROM_ALL)

add_executable(HelloWorld
	src/main.cpp
	src/config.h
	src/glad.c
	src/triangle_mesh.cpp
	src/triangle_mesh.h
)

target_include_directories(HelloWorld PRIVATE dep/include)

# target_link_libraries(HelloWorld
# 	glfw
# 	OpenGL::GL
# )

target_link_libraries(HelloWorld
	glfw
	OpenGL::GL

)

# add_executable(HelloWorld src/main.cpp src/config.h)

# target_include_directories(HelloWorld PRIVATE dep/include)

# # link_directories(${CMAKE_SOURCE_DIR}/dep/lib)

# # target_link_libraries(HelloWorld PRIVATE glfw3)
# target_link_libraries(HelloWorld PRIVATE ${CMAKE_SOURCE_DIR}/dep/lib/glfw3.lib)
