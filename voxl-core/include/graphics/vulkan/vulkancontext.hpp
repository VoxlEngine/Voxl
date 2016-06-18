#ifndef VOXL_GRAPHICS_GRAPHICS_CONTEXT_H_
#define VOXL_GRAPHICS_GRAPHICS_CONTEXT_H_

#define VULKAN_VERSION_MAJOR 1
#define VULKAN_VERSION_MINOR 0
#define VULKAN_VERSION_PATCH 8
#include <vulkan/vulkan.h>

#include "voxl.hpp"
#include <GLFW/glfw3.h>
#include <vector>

#include "config.hpp"

namespace voxl {
namespace graphics {
namespace vulkan {
class VulkanContext {
public:
  bool Init(Config config);
  void Destroy();
  void Swap();
  bool Clear();

  GLFWwindow *window;

  VkQueue graphicsQueue;
  VkQueue presentQueue;
  uint32_t graphicsQueueIndex;
  uint32_t presentQueueIndex;

  VkCommandPool commandPool;

  bool ready;

private:
  bool CreateSwapchain(const char *title, int width, int height);
  bool CreateCommandPool();
  bool CreateSemaphores();

  VkInstance instance;

  VkPhysicalDevice physDev;
  VkDevice dev;

  VkSurfaceKHR surf;
  VkSwapchainKHR swapchain;

  VkSemaphore acquireCompleteSemaphore;
  VkSemaphore renderCompleteSemaphore;
};
}
}
}

#endif
