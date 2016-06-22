#ifndef VOXL_GRAPHICS_VK_CONTEXT_H_
#define VOXL_GRAPHICS_VK_CONTEXT_H_

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
  void StartFrame();
  void EndFrame();

  inline VkDevice GetDevice() { return dev; };
  inline VkInstance GetInstance() { return instance; };
  inline VkSwapchainKHR GetSwapchain() { return swapchain; };

  GLFWwindow *window;

  VkQueue queue;
  uint32_t graphicsQueueIndex;
  uint32_t presentQueueIndex;

  uint32_t currentImage;

  std::vector<VkImage> swapchainImages;

  VkCommandPool commandPool;

  VkCommandBuffer setupCmdBuffer;
  VkCommandBuffer prePresentCmdBuffer;
  VkCommandBuffer postPresentCmdBuffer;
  std::vector<VkCommandBuffer> drawCmdBuffers;

private:
  bool CreateSwapchain(const char *title, int width, int height);
  bool CreateCommandPool();
  bool CreateSetupCommandBuffer();
  bool FlushSetupCommandBuffer();
  bool CreatePresentCommandBuffers();
  bool CreateDrawCommandBuffers();
  bool PrePresentBarrier(VkImage image);
  bool PostPresentBarrier(VkImage image);

  VkInstance instance;

  VkPhysicalDevice physDev;
  VkDevice dev;

  VkSurfaceKHR surf;
  VkSwapchainKHR swapchain = NULL;

  VkSemaphore acquireCompleteSemaphore;
  VkSemaphore renderCompleteSemaphore;
};
}
}
}

#endif
