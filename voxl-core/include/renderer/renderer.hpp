#ifndef VOXL_RENDERER_RENDERER_H_
#define VOXL_RENDERER_RENDERER_H_

#define VULKAN_VERSION_MAJOR 1
#define VULKAN_VERSION_MINOR 0
#define VULKAN_VERSION_PATCH 1
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include "config.hpp"
#include "voxl.hpp"

namespace voxl {
namespace renderer {
class Renderer {
public:
  Renderer(Config config);
  ~Renderer();

  inline GLFWwindow *getWindow() { return window; };

private:
  VkInstance instance;
  VkDevice dev;
  VkSurfaceKHR surf;

  GLFWwindow *window;
};
}
}

#endif
