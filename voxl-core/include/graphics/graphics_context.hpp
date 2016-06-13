#ifndef VOXL_GRAPHICS_GRAPHICS_CONTEXT_H_
#define VOXL_GRAPHICS_GRAPHICS_CONTEXT_H_

#define VULKAN_VERSION_MAJOR 1
#define VULKAN_VERSION_MINOR 0
#define VULKAN_VERSION_PATCH 1
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include "config.hpp"
#include "voxl.hpp"

namespace voxl {
namespace graphics {
class GraphicsContext {
public:
  int Init(Config config);
  void Destroy();
  void Swap();

  GLFWwindow *window;

private:
  VkInstance instance;

  VkPhysicalDevice physDev;
  VkDevice dev;

  VkSurfaceKHR surf;
  VkSwapchainKHR swapchain;

  VkSemaphore imageAvailableSemaphore;
  VkSemaphore renderingFinishedSemaphore;

  VkQueue graphicsQueue;
  VkQueue presentQueue;
};
}
}

#endif
