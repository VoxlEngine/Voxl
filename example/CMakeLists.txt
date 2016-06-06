project(example)

include_directories(.)
add_executable(example
  main.cpp)
target_link_libraries(example voxl ${VULKAN_LIBRARY})
