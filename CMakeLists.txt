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
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT")
# # target_compile_definitions(HelloWorld PRIVATE -D_ITERATOR_DEBUG_LEVEL=0)

# if(WIN32)
# 	message(STATUS "Windows detected")
# 	add_definitions(-DUNICODE -D_UNICODE)
#     # set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
#     set(CMAKE_CXX_FLAGS_RELEASE "/MT") # for static libs
#     set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
# endif(WIN32)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# looking for opengl
set(OpenGL_GL_PREFERENCE "GLVND" CACHE STRING "OpenGL preference")
message(STATUS "Looking for OpenGL")
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
	message(STATUS "OpenGL found")
else()
	message(FATAL_ERROR "OpenGL not found")
endif(OPENGL_FOUND)


# find_package(OpenEXR QUIET)
# if(NOT OpenEXR_FOUND)
# 	include(ExternalProject)

# 	ExternalProject_Add(
# 	openexr
# 	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/openexr
# 	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/openexr
# 	CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release
# 				-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/dep/openexr/build/install-dir
# 				-DBUILD_TESTING=OFF
# 				-DBUILD_SHARED_LIBS=OFF
# 				-DOPENEXR_BUILD_TOOLS=OFF
# 				-DOPENEXR_INSTALL_TOOLS=OFF
# 				-DOPENEXR_INSTALL_EXAMPLES=OFF
# 	)
# 	list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR}/dep/openexr/install-dir/lib/cmake/)
# 	# Now try finding OpenEXR again
# 	find_package(OpenEXR REQUIRED)
# 	if(OpenEXR_FOUND)
# 		message(STATUS "OpenEXR found")
# 	else()
# 		message(FATAL_ERROR "OpenEXR not found")
# 	endif()
	
# 	# Optionally, if you want to depend on the openexr target
# 	# add_dependencies(YourTarget openexr)
# else()
# 	message(STATUS "OpenEXR found")
# endif()

# find_package(OpenEXR QUIET)
# if(NOT OpenEXR_FOUND)
#   message(STATUS "OpenEXR not found, building from source")
# else()
#   message(STATUS "OpenEXR found")
# endif()





# list(APPEND CMAKE_PREFIX_PATH "/home/imisumi/sgoinfre/openexr/build/install/lib/cmake/")
# list(APPEND CMAKE_PREFIX_PATH "dep/openexr/build/install-dir/lib/cmake/")
# find_package(OpenEXR REQUIRED)
# if(OpenEXR_FOUND)
# 	message(STATUS "OpenEXR found")
# else()
# 	message(FATAL_ERROR "OpenEXR not found")
# endif()


find_package(OpenEXR QUIET)
if(OpenEXR_FOUND)
	message(STATUS "OpenEXR found")
else()
	message(STATUS "OpenEXR not found, building from source")

	# Specify the path to the install.bat script
	set(INSTALL_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/install.bat")

	# Run the install.bat script using execute_process
	execute_process(COMMAND cmd /c "${INSTALL_SCRIPT}"
					WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
					RESULT_VARIABLE CMD_RESULT)

	# Add the install-dir to the CMAKE_PREFIX_PATH
	list(APPEND CMAKE_PREFIX_PATH "dep/openexr/build/install-dir/lib/cmake/")
	find_package(OpenEXR REQUIRED)
	if(CMD_RESULT EQUAL 0)
		message(STATUS "Installation successful after building from source")
	else()
		message(FATAL_ERROR "Error running installation script")
	endif()
endif()


add_subdirectory(dep/glfw EXCLUDE_FROM_ALL)

add_executable(HelloWorld
	src/main.cpp
	src/config.h
	src/glad.c
	src/triangle_mesh.cpp
	src/triangle_mesh.h
)

target_include_directories(HelloWorld PRIVATE 
	dep/include
	dep/openexr/build/install/include
	dep/openexr/build/install/include/Imath
	dep/openexr/build/install/include/OpenEXR
	# /home/imisumi/sgoinfre/openexr/build/install/include
	# /home/imisumi/sgoinfre/openexr/build/install/include/Imath
	# /home/imisumi/sgoinfre/openexr/build/install/include/OpenEXR
)


target_link_libraries(HelloWorld
	glfw
	OpenGL::GL
	# OpenEXR::IlmImf
	OpenEXR::OpenEXR
)
