cmake_minimum_required(VERSION 2.8)

project(voxl)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include_directories(voxl-core/include)

add_subdirectory(voxl-core)
add_subdirectory(example)
