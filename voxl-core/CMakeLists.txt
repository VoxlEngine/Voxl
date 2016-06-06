project(voxl-core)

find_package(Vulkan REQUIRED)

add_library(voxl
  src/renderer/renderer.cpp)

include_directories(voxl ${VULKAN_INCLUDE_DIR})
