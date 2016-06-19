cmake_minimum_required(VERSION 2.8)

project(voxl)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(Vulkan REQUIRED)
include_directories(voxl ${VULKAN_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
include_directories(voxl ${OPENGL_INCLUDE_DIR})

include_directories(voxl-core/include deps/glfw/include)

add_subdirectory(deps/glfw)

add_subdirectory(voxl-core)
add_subdirectory(example)
