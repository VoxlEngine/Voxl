#include <cstring>
#include <iostream>
#include <vector>

#include "graphics/vk/util.hpp"
#include "graphics/vk/vkcontext.hpp"

namespace voxl {
namespace graphics {
namespace vk {
// TODO: Resizing
bool VkContext::Init(Config config) {
  if (!CreateSwapchain(config.windowTitle, config.windowWidth,
                       config.windowHeight)) {
    std::cout << "Unable to create swapchain" << std::endl;
  }

  if (!CreateSemaphores()) {
    std::cout << "Unable to create semaphores" << std::endl;
    return false;
  };

  if (!CreateCommandBuffers()) {
    std::cout << "Unable to create command buffers" << std::endl;
    return false;
  };

  // Record command buffers
  /*uint32_t imageCount = static_cast<uint32_t>(graphicsCmdBuffers.size());

  std::vector<VkImage> swapchainImages(imageCount);
  if (vkGetSwapchainImagesKHR(dev, swapchain, &imageCount,
                              &swapchainImages[0]) != VK_SUCCESS) {
    std::cout << "Unable to get swapchain images" << std::endl;
    return false;
  }

  VkCommandBufferBeginInfo beginInfo;
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.pNext = NULL;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  beginInfo.pInheritanceInfo = NULL;

  VkClearColorValue clearColor = {{1.0f, 0.8f, 0.4f, 0.0f}};

  VkImageSubresourceRange subresourceRange;
  subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  subresourceRange.baseMipLevel = 0;
  subresourceRange.levelCount = 1;
  subresourceRange.baseArrayLayer = 0;
  subresourceRange.layerCount = 1;

  for (uint32_t i = 0; i < imageCount; i++) {
    // Begin command buffer
    vkBeginCommandBuffer(graphicsCmdBuffers[i], &beginInfo);

    // Set the image layout
    SetImageLayout(graphicsCmdBuffers[i], swapchainImages[i],
                   subresourceRange, VK_IMAGE_LAYOUT_UNDEFINED,
                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    // Clear the image
    vkCmdClearColorImage(graphicsCmdBuffers[i], swapchainImages[i],
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1,
                         &subresourceRange);

    // Set the image layout back to undefined
    SetImageLayout(graphicsCmdBuffers[i], swapchainImages[i],
                   subresourceRange, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                   VK_IMAGE_LAYOUT_UNDEFINED);

    // End command buffer
    if (vkEndCommandBuffer(graphicsCmdBuffers[i]) != VK_SUCCESS) {
      std::cout << "Unable to end command buffer" << std::endl;
      return false;
    }
  }*/

  return true;
}

void VkContext::Destroy() {
  vkDestroyInstance(instance, NULL);
  glfwTerminate();
}

void VkContext::Swap() {
  VkResult result;

  uint32_t imageIndex = 0;
  result = vkAcquireNextImageKHR(dev, swapchain, UINT64_MAX,
                                 acquireCompleteSemaphore, NULL, &imageIndex);
  switch (result) {
  case VK_SUCCESS:
  case VK_SUBOPTIMAL_KHR:
    break;
  case VK_ERROR_OUT_OF_DATE_KHR:
    std::cout << "Window size changed" << std::endl;
  default:
    std::cout << "Unable to get next swap chain image" << std::endl;
  }

  /*VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
  VkSubmitInfo submitInfo;
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.pNext = NULL;
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = &acquireCompleteSemaphore;
  submitInfo.pWaitDstStageMask = &waitDstStageMask;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &graphicsCmdBuffers[imageIndex];
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = &renderCompleteSemaphore;

  if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, NULL) != VK_SUCCESS) {
    std::cout << "Unable to submit queues" << std::endl;
    return;
  }*/

  VkPresentInfoKHR presentInfo;
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.pNext = NULL;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = &acquireCompleteSemaphore;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &swapchain;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = NULL;

  result = vkQueuePresentKHR(presentQueue, &presentInfo);

  switch (result) {
  case VK_SUCCESS:
    break;
  case VK_ERROR_OUT_OF_DATE_KHR:
  case VK_SUBOPTIMAL_KHR:
    std::cout << "Window size changed" << std::endl;
  default:
    std::cout << "Unable to present swap chain image" << std::endl;
  }

  vkDeviceWaitIdle(dev);
}

bool VkContext::CreateSwapchain(const char *title, int width, int height) {
  // Initialize GLFW
  if (!glfwInit()) {
    std::cout << "Unable to initialize GLFW" << std::endl;
    return false;
  }

  // Check if GLFW supports Vulkan
  if (!glfwVulkanSupported()) {
    std::cout << "GLFW does not support Vulkan" << std::endl;
    return false;
  }

  uint32_t numExtensions = 0;
  const char **extensionNames =
      glfwGetRequiredInstanceExtensions(&numExtensions);

  VkApplicationInfo appCreateInfo;
  appCreateInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appCreateInfo.pNext = NULL;
  appCreateInfo.pApplicationName = title;
  appCreateInfo.pEngineName = "Voxl";
  appCreateInfo.engineVersion = VK_MAKE_VERSION(
      VOXL_VERSION_MAJOR, VOXL_VERSION_MINOR, VOXL_VERSION_PATCH);
  appCreateInfo.apiVersion = VK_MAKE_VERSION(
      VULKAN_VERSION_MAJOR, VULKAN_VERSION_MINOR, VULKAN_VERSION_PATCH);

  VkInstanceCreateInfo instanceCreateInfo;
  instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceCreateInfo.pNext = NULL;
  instanceCreateInfo.flags = 0;
  instanceCreateInfo.pApplicationInfo = &appCreateInfo;
  instanceCreateInfo.enabledLayerCount = 0;
  instanceCreateInfo.ppEnabledLayerNames = NULL;
  instanceCreateInfo.enabledExtensionCount = numExtensions;
  instanceCreateInfo.ppEnabledExtensionNames = extensionNames;

  // Create instance
  if (vkCreateInstance(&instanceCreateInfo, NULL, &instance) != VK_SUCCESS) {
    std::cout << "Unable to create Vulkan instance: Error code " << std::endl;
    return false;
  }

  // Enumerate physical devices (GPUs)
  uint32_t physDevCount = 0;

  if (vkEnumeratePhysicalDevices(instance, &physDevCount, NULL) != VK_SUCCESS) {
    std::cout << "Unable to query number of GPUs" << std::endl;
    return false;
  }

  if (physDevCount == 0) {
    std::cout << "No compatilble Vulkan GPU" << std::endl;
    return false;
  }

  std::vector<VkPhysicalDevice> physDevs(physDevCount);
  if (vkEnumeratePhysicalDevices(instance, &physDevCount, &physDevs[0]) !=
      VK_SUCCESS) {
    std::cout << "Unable to get list of GPUs" << std::endl;
    return false;
  }

  // Print physical device info
  VkPhysicalDeviceProperties deviceProperties;
  for (uint32_t i = 0; i < physDevCount; i++) {
    memset(&deviceProperties, 0, sizeof(deviceProperties));
    vkGetPhysicalDeviceProperties(physDevs[i], &deviceProperties);
    std::cout << deviceProperties.deviceName << std::endl;
    std::cout << "   Driver Version: "
              << (deviceProperties.driverVersion >> 22 & 0x3ff) << "."
              << (deviceProperties.driverVersion >> 12 & 0x3ff) << "."
              << (deviceProperties.driverVersion & 0xfff) << std::endl;
    std::cout << "   API Version: "
              << (deviceProperties.apiVersion >> 22 & 0x3ff) << "."
              << (deviceProperties.apiVersion >> 12 & 0x3ff) << "."
              << (deviceProperties.apiVersion & 0xfff) << std::endl;
  }

  // Get queue families
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physDevs[0], &queueFamilyCount,
                                           NULL);
  std::vector<VkQueueFamilyProperties> familyProperties(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(physDevs[0], &queueFamilyCount,
                                           &familyProperties[0]);

  std::vector<VkBool32> supportsPresent(queueFamilyCount);

  for (uint32_t i = 0; i < queueFamilyCount; i++) {
    vkGetPhysicalDeviceSurfaceSupportKHR(physDevs[0], i, surf,
                                         &supportsPresent[i]);
  }

  // TODO: Support separate graphics and present queues

  // Get a queue with support for both graphics and present
  graphicsQueueIndex = UINT32_MAX;
  presentQueueIndex = UINT32_MAX;
  for (uint32_t i = 0; i < queueFamilyCount; i++) {
    if (familyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      if (graphicsQueueIndex == UINT32_MAX)
        graphicsQueueIndex = i;
      if (supportsPresent[i] == VK_TRUE) {
        graphicsQueueIndex = i;
        presentQueueIndex = i;
        break;
      }
    }
  }

  if (graphicsQueueIndex == UINT32_MAX || presentQueueIndex == UINT32_MAX) {
    std::cout
        << "Unable to find queue which supports both present and graphics "
        << std::endl;
    return false;
  }

  // Create queues
  float queuePriorities[] = {1.0f};

  VkDeviceQueueCreateInfo queueCreateInfo;
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.pNext = NULL;
  queueCreateInfo.flags = 0;
  queueCreateInfo.queueFamilyIndex = graphicsQueueIndex;
  queueCreateInfo.queueCount = 1;
  queueCreateInfo.pQueuePriorities = queuePriorities;

  // Create device
  VkDeviceCreateInfo devCreateInfo;
  devCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  devCreateInfo.pNext = NULL;
  devCreateInfo.flags = 0;

  devCreateInfo.enabledLayerCount = 0;
  devCreateInfo.ppEnabledLayerNames = NULL;
  devCreateInfo.enabledExtensionCount = 0;
  devCreateInfo.ppEnabledExtensionNames = NULL;
  devCreateInfo.pEnabledFeatures = NULL;

  devCreateInfo.queueCreateInfoCount = 1;
  devCreateInfo.pQueueCreateInfos = &queueCreateInfo;

  if (vkCreateDevice(physDevs[0], &devCreateInfo, NULL, &dev) != VK_SUCCESS) {
    std::cout << "Unabled to create Vulkan device" << std::endl;
    return false;
  }

  // Get queues
  vkGetDeviceQueue(dev, graphicsQueueIndex, 0, &graphicsQueue);
  vkGetDeviceQueue(dev, presentQueueIndex, 0, &presentQueue);

  // Create window
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window = glfwCreateWindow(width, height, title, NULL, NULL);

  if (window == NULL) {
    std::cout << "Unable to create GLFW window" << std::endl;
  }

  // Create surface
  if (glfwCreateWindowSurface(instance, window, NULL, &surf) != VK_SUCCESS) {
    std::cout << "Unable to create window Vulkan surface" << std::endl;
    return false;
  }

  // Get surface capabilities
  VkSurfaceCapabilitiesKHR surfCapabilities;
  if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
          physDevs[0], surf, &surfCapabilities) != VK_SUCCESS) {
    std::cout << "Unable to get device surface capabilities" << std::endl;
    return false;
  }

  // Get supported formats
  uint32_t formatCount = 0;
  if (vkGetPhysicalDeviceSurfaceFormatsKHR(physDevs[0], surf, &formatCount,
                                           NULL) != VK_SUCCESS) {
    std::cout << "Unable to get device surface formats" << std::endl;
    return false;
  }

  std::vector<VkSurfaceFormatKHR> formats(formatCount);
  if (vkGetPhysicalDeviceSurfaceFormatsKHR(physDevs[0], surf, &formatCount,
                                           &formats[0]) != VK_SUCCESS) {
    std::cout << "Unable to get device surface formats" << std::endl;
    return false;
  }

  // Select best format
  VkFormat format;
  VkColorSpaceKHR colorSpace = formats[0].colorSpace;

  if (formatCount == 1 && formats[0].format == VK_FORMAT_UNDEFINED) {
    format = VK_FORMAT_R8G8B8A8_UNORM;
  } else if (formatCount >= 1) {
    format = formats[0].format;
  } else {
    std::cout << "Device surface does not support any color format"
              << std::endl;
    return false;
  }

  // Get swap chain image count
  uint32_t swapchainImageCount = surfCapabilities.minImageCount + 1;
  if (swapchainImageCount > surfCapabilities.maxImageCount) {
    swapchainImageCount = surfCapabilities.maxImageCount;
  }

  // Get swap chain usage flags
  VkImageUsageFlags swapchainUsageFlags;

  if (surfCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
    swapchainUsageFlags =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  } else {
    std::cout << "Swap chain transfer DST not supported" << std::endl;
    return false;
  }

  // Get present modes
  uint32_t presentModeCount = 0;
  if (vkGetPhysicalDeviceSurfacePresentModesKHR(
          physDevs[0], surf, &presentModeCount, NULL) != VK_SUCCESS ||
      presentModeCount == 0) {
    std::cout << "Unable to get device surface present modes" << std::endl;
    return false;
  }

  std::vector<VkPresentModeKHR> presentModes(presentModeCount);

  if (vkGetPhysicalDeviceSurfacePresentModesKHR(
          physDevs[0], surf, &presentModeCount, &presentModes[0]) !=
      VK_SUCCESS) {
    std::cout << "Unable to get device surface present modes" << std::endl;
    return false;
  }

  VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

  // Select Mailbox mode if possible, otherwise select FIFO (should always be
  // supported)
  for (uint32_t i = 0; i < presentModes.size(); i++) {
    if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
      presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
      break;
    }
  }

  // Select identity transform mode if possible, otherwise use
  // currentTransform
  VkSurfaceTransformFlagBitsKHR transform;
  if (surfCapabilities.supportedTransforms &
      VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
    transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  } else {
    transform = surfCapabilities.currentTransform;
  }

  // Create swapchain
  VkSwapchainCreateInfoKHR swapchainCreateInfo;
  swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchainCreateInfo.pNext = NULL;
  swapchainCreateInfo.flags = 0;
  swapchainCreateInfo.surface = surf;
  swapchainCreateInfo.minImageCount = swapchainImageCount;
  swapchainCreateInfo.imageFormat = format;
  swapchainCreateInfo.imageColorSpace = colorSpace;
  swapchainCreateInfo.imageExtent = surfCapabilities.currentExtent;
  swapchainCreateInfo.imageArrayLayers = 1;
  swapchainCreateInfo.imageUsage = swapchainUsageFlags;
  swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  swapchainCreateInfo.queueFamilyIndexCount = 0;
  swapchainCreateInfo.pQueueFamilyIndices = NULL;
  swapchainCreateInfo.preTransform = transform;
  swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapchainCreateInfo.presentMode = presentMode;
  swapchainCreateInfo.clipped = VK_TRUE;
  swapchainCreateInfo.oldSwapchain = NULL;

  if (vkCreateSwapchainKHR(dev, &swapchainCreateInfo, NULL, &swapchain) !=
      VK_SUCCESS) {
    std::cout << "Unable to create swapchain" << std::endl;
    return false;
  }

  return true;
}

bool VkContext::CreateSemaphores() {
  // Create semaphores
  VkSemaphoreCreateInfo semaphoreCreateInfo;
  semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  semaphoreCreateInfo.pNext = NULL;
  semaphoreCreateInfo.flags = 0;
  if (vkCreateSemaphore(dev, &semaphoreCreateInfo, NULL,
                        &acquireCompleteSemaphore) != VK_SUCCESS) {
    std::cout << "Unable to create semaphore" << std::endl;
    return false;
  }
  if (vkCreateSemaphore(dev, &semaphoreCreateInfo, NULL,
                        &renderCompleteSemaphore) != VK_SUCCESS) {
    std::cout << "Unable to create semaphore" << std::endl;
    return false;
  }

  return true;
}

bool VkContext::CreateCommandBuffers() {
  // Create command pool for buffer allocation
  VkCommandPoolCreateInfo commandPoolCreateInfo;
  commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  commandPoolCreateInfo.pNext = NULL;
  commandPoolCreateInfo.flags = 0;
  commandPoolCreateInfo.queueFamilyIndex = presentQueueIndex;

  if (vkCreateCommandPool(dev, &commandPoolCreateInfo, NULL, &commandPool) !=
      VK_SUCCESS) {
    std::cout << "Unable to create present command pool" << std::endl;
    return false;
  }

  uint32_t imageCount = 0;
  if (vkGetSwapchainImagesKHR(dev, swapchain, &imageCount, NULL) !=
      VK_SUCCESS) {
    std::cout << "Unable to get number of swapchain images" << std::endl;
    return false;
  }

  graphicsCmdBuffers.resize(imageCount);

  VkCommandBufferAllocateInfo commandBufferAllocateInfo;
  commandBufferAllocateInfo.sType =
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferAllocateInfo.pNext = NULL;
  commandBufferAllocateInfo.commandPool = commandPool;
  commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  commandBufferAllocateInfo.commandBufferCount = imageCount;

  if (vkAllocateCommandBuffers(dev, &commandBufferAllocateInfo,
                               &graphicsCmdBuffers[0]) != VK_SUCCESS) {
    std::cout << "Unable to allocate command buffers" << std::endl;
    return false;
  }

  return true;
}
}
}
}
