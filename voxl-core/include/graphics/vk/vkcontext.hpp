#ifndef VOXL_GRAPHICS_VULKAN_CONTEXT_H_
#define VOXL_GRAPHICS_VULKAN_CONTEXT_H_

#define VULKAN_VERSION_MAJOR 1
#define VULKAN_VERSION_MINOR 0
#define VULKAN_VERSION_PATCH 8
#include <vulkan/vulkan.h>

#include "voxl.hpp"
#include <GLFW/glfw3.h>
#include <vector>

#include "config.hpp"

#include "graphics/context.hpp"

namespace voxl {
namespace graphics {
namespace vk {
class VkContext : public Context {
public:
  bool Init(Config config);
  void Destroy();
  void Swap();

  GLFWwindow *window;

  VkQueue graphicsQueue;
  VkQueue presentQueue;
  uint32_t graphicsQueueIndex;
  uint32_t presentQueueIndex;

  VkCommandPool commandPool;

  std::vector<VkCommandBuffer> graphicsCmdBuffers;

private:
  bool CreateSwapchain(const char *title, int width, int height);
  bool CreateCommandBuffers();
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
