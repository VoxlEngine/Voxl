#include <cstring>
#include <iostream>
#include <vector>

#include "graphics/vk/util.hpp"
#include "graphics/vk/vkcontext.hpp"

namespace voxl {
namespace graphics {
namespace vk {
#ifdef VOXL_DEBUG
PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallback = VK_NULL_HANDLE;
PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallback = VK_NULL_HANDLE;
PFN_vkDebugReportMessageEXT dbgBreakCallback = VK_NULL_HANDLE;

VkDebugReportCallbackEXT msgCallback;

VkBool32 messageCallback(VkDebugReportFlagsEXT flags,
                         VkDebugReportObjectTypeEXT objType, uint64_t srcObject,
                         size_t location, int32_t msgCode,
                         const char *pLayerPrefix, const char *pMsg,
                         void *pUserData) {
  std::string text(pMsg);

  std::string prefix("");

  if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
    prefix += "ERROR ";
  };

  if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
    prefix += "WARNING ";
  };

  if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
    prefix += "PERFORMANCE ";
  };

  if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
    prefix += "INFO ";
  }

  if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
    prefix += "DEBUG ";
  }

  std::cout << prefix << "at " << location << " [" << pLayerPrefix << "] Code "
            << msgCode << " : " << pMsg << std::endl;

  return VK_FALSE;
}
#endif

// TODO: Resizing
bool VkContext::Init(Config config) {
  if (!CreateSwapchain(config.windowTitle, config.windowWidth,
                       config.windowHeight)) {
    std::cout << "Unable to create swapchain" << std::endl;
  }

  if (!CreateCommandPool()) {
    std::cout << "Unable to create command pool" << std::endl;
    return false;
  }

  if (!CreateSetupCommandBuffer()) {
    std::cout << "Unable to create setup command buffer" << std::endl;
    return false;
  }

  if (!CreatePresentCommandBuffers()) {
    std::cout << "Unable to create present command buffers" << std::endl;
    return false;
  }

  if (!CreateDrawCommandBuffers()) {
    std::cout << "Unable to create draw command buffers" << std::endl;
    return false;
  }

  if (!FlushSetupCommandBuffer()) {
    std::cout << "Unable to flush setup command buffer" << std::endl;
    return false;
  }

  return true;
}

void VkContext::Destroy() {
  // Destroy semaphores
  vkDestroySemaphore(dev, acquireCompleteSemaphore, NULL);
  vkDestroySemaphore(dev, renderCompleteSemaphore, NULL);

  // Free command buffers
  if (drawCmdBuffers.size() > 0) {
    vkFreeCommandBuffers(dev, commandPool, (uint32_t)drawCmdBuffers.size(),
                         &drawCmdBuffers[0]);
  }

  // Free present command buffers
  vkFreeCommandBuffers(dev, commandPool, 1, &prePresentCmdBuffer);
  vkFreeCommandBuffers(dev, commandPool, 1, &postPresentCmdBuffer);

  // Destroy command pool
  vkDestroyCommandPool(dev, commandPool, NULL);

  // Destroy swapchain
  vkDestroySwapchainKHR(dev, swapchain, NULL);

  // Destroy surface
  vkDestroySurfaceKHR(instance, surf, NULL);

  // Destroy device
  vkDestroyDevice(dev, NULL);

#ifdef VOXL_DEBUG
  // Destroy debug report callback
  DestroyDebugReportCallback(instance, msgCallback, NULL);
#endif

  // Destroy instance
  vkDestroyInstance(instance, NULL);

  // Terminate GLFW
  glfwTerminate();
}

void VkContext::StartFrame() {
  switch (vkAcquireNextImageKHR(dev, swapchain, UINT64_MAX,
                                acquireCompleteSemaphore, NULL,
                                &currentImage)) {
  case VK_SUCCESS:
  case VK_SUBOPTIMAL_KHR:
    break;
  case VK_ERROR_OUT_OF_DATE_KHR:
    std::cout << "Window size changed" << std::endl;
  default:
    std::cout << "Unable to get next swap chain image" << std::endl;
  }

  PostPresentBarrier(swapchainImages[currentImage]);
}

void VkContext::EndFrame() {
  PrePresentBarrier(swapchainImages[currentImage]);

  VkPresentInfoKHR presentInfo;
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.pNext = NULL;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = &acquireCompleteSemaphore;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &swapchain;
  presentInfo.pImageIndices = &currentImage;
  presentInfo.pResults = NULL;

  switch (vkQueuePresentKHR(queue, &presentInfo)) {
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

  std::vector<const char *> enabledLayers;
  std::vector<const char *> enabledExtensions;

#ifdef VOXL_DEBUG
  enabledLayers.push_back("VK_LAYER_LUNARG_standard_validation");

  enabledExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif

  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  for (int i = 0; i < glfwExtensionCount; i++) {
    enabledExtensions.push_back(glfwExtensions[i]);
  }

  enabledExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

  VkApplicationInfo appCreateInfo = {};
  appCreateInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appCreateInfo.pNext = NULL;
  appCreateInfo.pApplicationName = title;
  appCreateInfo.pEngineName = "Voxl";
  appCreateInfo.engineVersion = VK_MAKE_VERSION(
      VOXL_VERSION_MAJOR, VOXL_VERSION_MINOR, VOXL_VERSION_PATCH);
  appCreateInfo.apiVersion = VK_MAKE_VERSION(
      VULKAN_VERSION_MAJOR, VULKAN_VERSION_MINOR, VULKAN_VERSION_PATCH);

  VkInstanceCreateInfo instanceCreateInfo = {};
  instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceCreateInfo.pNext = NULL;
  instanceCreateInfo.flags = 0;
  instanceCreateInfo.pApplicationInfo = &appCreateInfo;
  instanceCreateInfo.enabledLayerCount = (uint32_t)enabledLayers.size();
  instanceCreateInfo.ppEnabledLayerNames = &enabledLayers[0];
  instanceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
  instanceCreateInfo.ppEnabledExtensionNames = &enabledExtensions[0];

  // Create instance
  if (vkCreateInstance(&instanceCreateInfo, NULL, &instance) != VK_SUCCESS) {
    std::cout << "Unable to create Vulkan instance" << std::endl;
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
  VkPhysicalDeviceProperties deviceProperties = {};
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

  VkDeviceQueueCreateInfo queueCreateInfo = {};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.pNext = NULL;
  queueCreateInfo.flags = 0;
  queueCreateInfo.queueFamilyIndex = graphicsQueueIndex;
  queueCreateInfo.queueCount = 1;
  queueCreateInfo.pQueuePriorities = queuePriorities;

  // Create device
  std::vector<const char *> enabledDevExtensions;
  enabledDevExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

  VkDeviceCreateInfo devCreateInfo = {};
  devCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  devCreateInfo.pNext = NULL;
  devCreateInfo.flags = 0;

  devCreateInfo.enabledLayerCount = (uint32_t)enabledLayers.size();
  devCreateInfo.ppEnabledLayerNames = &enabledLayers[0];
  devCreateInfo.enabledExtensionCount = (uint32_t)enabledDevExtensions.size();
  devCreateInfo.ppEnabledExtensionNames = &enabledDevExtensions[0];
  devCreateInfo.pEnabledFeatures = NULL;

  devCreateInfo.queueCreateInfoCount = 1;
  devCreateInfo.pQueueCreateInfos = &queueCreateInfo;

  if (vkCreateDevice(physDevs[0], &devCreateInfo, NULL, &dev) != VK_SUCCESS) {
    std::cout << "Unabled to create Vulkan device" << std::endl;
    return false;
  }

  // Create semaphores
  VkSemaphoreCreateInfo semaphoreCreateInfo = {};
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

#ifdef VOXL_DEBUG
  // Set debug validation callback
  CreateDebugReportCallback =
      (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(
          instance, "vkCreateDebugReportCallbackEXT");
  DestroyDebugReportCallback =
      (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(
          instance, "vkDestroyDebugReportCallbackEXT");
  dbgBreakCallback = (PFN_vkDebugReportMessageEXT)vkGetInstanceProcAddr(
      instance, "vkDebugReportMessageEXT");

  VkDebugReportCallbackCreateInfoEXT dbgCreateInfo = {};
  dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
  dbgCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)messageCallback;
  dbgCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
                        VK_DEBUG_REPORT_WARNING_BIT_EXT |
                        VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;

  if (CreateDebugReportCallback(instance, &dbgCreateInfo, NULL, &msgCallback) !=
      VK_SUCCESS) {
    std::cout << "Unable to create debug report callback" << std::endl;
  }
#endif

  // Get queues
  vkGetDeviceQueue(dev, graphicsQueueIndex, 0, &queue);
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
  VkSurfaceCapabilitiesKHR surfCapabilities = {};
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

  // Make sure presentation is supported
  VkBool32 supported = false;
  if (vkGetPhysicalDeviceSurfaceSupportKHR(physDevs[0], presentQueueIndex, surf,
                                           &supported) != VK_SUCCESS) {
    std::cout << "Unable to query for GPU surface support" << std::endl;
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
  VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
  swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchainCreateInfo.pNext = NULL;
  swapchainCreateInfo.flags = 0;
  swapchainCreateInfo.surface = surf;
  swapchainCreateInfo.minImageCount = swapchainImageCount;
  swapchainCreateInfo.imageFormat = format;
  swapchainCreateInfo.imageColorSpace = colorSpace;
  swapchainCreateInfo.imageExtent = surfCapabilities.currentExtent;
  swapchainCreateInfo.imageArrayLayers = 1;
  swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  swapchainCreateInfo.queueFamilyIndexCount = 0;
  swapchainCreateInfo.pQueueFamilyIndices = NULL;
  swapchainCreateInfo.preTransform = transform;
  swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapchainCreateInfo.presentMode = presentMode;
  swapchainCreateInfo.clipped = VK_TRUE;
  swapchainCreateInfo.oldSwapchain = swapchain;

  VkResult result =
      vkCreateSwapchainKHR(dev, &swapchainCreateInfo, NULL, &swapchain);
  if (result != VK_SUCCESS) {
    std::cout << "Unable to create swapchain" << result << std::endl;
    return false;
  }

  // Get number of swapchain images
  uint32_t imageCount = 0;
  if (vkGetSwapchainImagesKHR(dev, swapchain, &imageCount, NULL) !=
      VK_SUCCESS) {
    std::cout << "Unable to get number of swapchain images" << std::endl;
    return false;
  }

  // Get swapchain images
  swapchainImages.resize(imageCount);
  if (vkGetSwapchainImagesKHR(dev, swapchain, &imageCount,
                              &swapchainImages[0]) != VK_SUCCESS) {
    std::cout << "Unable to get swapchain images" << std::endl;
    return false;
  }

  return true;
}

bool VkContext::CreateCommandPool() {
  // Create command pool for buffer allocation
  VkCommandPoolCreateInfo commandPoolCreateInfo = {};
  commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  commandPoolCreateInfo.pNext = NULL;
  commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  commandPoolCreateInfo.queueFamilyIndex = graphicsQueueIndex;

  if (vkCreateCommandPool(dev, &commandPoolCreateInfo, NULL, &commandPool) !=
      VK_SUCCESS) {
    std::cout << "Unable to create present command pool" << std::endl;
    return false;
  }

  return true;
}

bool VkContext::CreateSetupCommandBuffer() {
  // Allocate setup command buffer
  VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
  commandBufferAllocateInfo.sType =
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferAllocateInfo.pNext = NULL;
  commandBufferAllocateInfo.commandPool = commandPool;
  commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  commandBufferAllocateInfo.commandBufferCount = 1;

  if (vkAllocateCommandBuffers(dev, &commandBufferAllocateInfo,
                               &setupCmdBuffer) != VK_SUCCESS) {
    std::cout << "Unable to allocate setup command buffer" << std::endl;
    return false;
  }

  // Build setup command buffer
  VkCommandBufferBeginInfo beginInfo = {};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.pNext = NULL;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  beginInfo.pInheritanceInfo = NULL;

  VkImageSubresourceRange subresourceRange = {};
  subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  subresourceRange.baseMipLevel = 0;
  subresourceRange.levelCount = 1;
  subresourceRange.baseArrayLayer = 0;
  subresourceRange.layerCount = 1;

  VkImageMemoryBarrier memoryBarrier = {};
  memoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  memoryBarrier.pNext = NULL;
  memoryBarrier.srcAccessMask = 0;
  memoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
  memoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  memoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  memoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  memoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  memoryBarrier.subresourceRange = subresourceRange;
  memoryBarrier.image = NULL;

  vkBeginCommandBuffer(setupCmdBuffer, &beginInfo);

  for (int i = 0; i < swapchainImages.size(); i++) {
    memoryBarrier.image = swapchainImages[i];

    vkCmdPipelineBarrier(setupCmdBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                         VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, NULL, 0, NULL,
                         1, &memoryBarrier);
  }

  if (vkEndCommandBuffer(setupCmdBuffer) != VK_SUCCESS) {
    std::cout << "Unable to end command buffer" << std::endl;
    return false;
  }

  return true;
}

bool VkContext::FlushSetupCommandBuffer() {
  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.pNext = NULL;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &setupCmdBuffer;
  submitInfo.waitSemaphoreCount = 0;
  submitInfo.pWaitSemaphores = NULL;
  submitInfo.signalSemaphoreCount = 0;
  submitInfo.pSignalSemaphores = NULL;

  if (vkQueueSubmit(queue, 1, &submitInfo, NULL) != VK_SUCCESS) {
    std::cout << "Unable to submit queues" << std::endl;
    return false;
  }

  if (vkQueueWaitIdle(queue) != VK_SUCCESS) {
    std::cout << "Unable to wait for queue to idle" << std::endl;
    return false;
  }

  vkFreeCommandBuffers(dev, commandPool, 1, &setupCmdBuffer);

  return true;
}

bool VkContext::CreatePresentCommandBuffers() {
  VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
  commandBufferAllocateInfo.sType =
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferAllocateInfo.pNext = NULL;
  commandBufferAllocateInfo.commandPool = commandPool;
  commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  commandBufferAllocateInfo.commandBufferCount = 1;

  if (vkAllocateCommandBuffers(dev, &commandBufferAllocateInfo,
                               &prePresentCmdBuffer) != VK_SUCCESS) {
    std::cout << "Unable to allocate pre present command buffer" << std::endl;
    return false;
  }

  if (vkAllocateCommandBuffers(dev, &commandBufferAllocateInfo,
                               &postPresentCmdBuffer) != VK_SUCCESS) {
    std::cout << "Unable to allocate post present command buffer" << std::endl;
    return false;
  }

  return true;
}

bool VkContext::CreateDrawCommandBuffers() {
  // Create a draw command bufffer for each swapchain image
  drawCmdBuffers.resize(swapchainImages.size());

  VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
  commandBufferAllocateInfo.sType =
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferAllocateInfo.pNext = NULL;
  commandBufferAllocateInfo.commandPool = commandPool;
  commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  commandBufferAllocateInfo.commandBufferCount =
      (uint32_t)swapchainImages.size();

  if (vkAllocateCommandBuffers(dev, &commandBufferAllocateInfo,
                               &drawCmdBuffers[0]) != VK_SUCCESS) {
    std::cout << "Unable to allocate draw command buffers" << std::endl;
    return false;
  }

  return true;
}

bool VkContext::PrePresentBarrier(VkImage image) {
  VkCommandBufferBeginInfo beginInfo = {};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.pNext = NULL;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  beginInfo.pInheritanceInfo = NULL;

  if (vkBeginCommandBuffer(prePresentCmdBuffer, &beginInfo) != VK_SUCCESS) {
    std::cout << "Unable to begin command buffer" << std::endl;
  }

  VkImageSubresourceRange subresourceRange = {};
  subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  subresourceRange.baseMipLevel = 0;
  subresourceRange.levelCount = 1;
  subresourceRange.baseArrayLayer = 0;
  subresourceRange.layerCount = 1;

  VkImageMemoryBarrier memoryBarrier = {};
  memoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  memoryBarrier.pNext = NULL;
  memoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  memoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
  memoryBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  memoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  memoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  memoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  memoryBarrier.subresourceRange = subresourceRange;
  memoryBarrier.image = image;

  vkCmdPipelineBarrier(prePresentCmdBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                       VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, NULL, 0, NULL,
                       1, &memoryBarrier);

  if (vkEndCommandBuffer(prePresentCmdBuffer) != VK_SUCCESS) {
    std::cout << "Unable to end command buffer" << std::endl;
    return false;
  }

  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.pNext = NULL;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &prePresentCmdBuffer;
  submitInfo.waitSemaphoreCount = 0;
  submitInfo.pWaitSemaphores = NULL;
  submitInfo.signalSemaphoreCount = 0;
  submitInfo.pSignalSemaphores = NULL;

  if (vkQueueSubmit(queue, 1, &submitInfo, NULL) != VK_SUCCESS) {
    std::cout << "Unable to submit queues" << std::endl;
    return false;
  }

  return true;
}

bool VkContext::PostPresentBarrier(VkImage image) {
  VkCommandBufferBeginInfo beginInfo = {};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.pNext = NULL;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  beginInfo.pInheritanceInfo = NULL;

  if (vkBeginCommandBuffer(postPresentCmdBuffer, &beginInfo) != VK_SUCCESS) {
    std::cout << "Unable to begin command buffer" << std::endl;
  }

  VkImageSubresourceRange subresourceRange = {};
  subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  subresourceRange.baseMipLevel = 0;
  subresourceRange.levelCount = 1;
  subresourceRange.baseArrayLayer = 0;
  subresourceRange.layerCount = 1;

  VkImageMemoryBarrier memoryBarrier = {};
  memoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  memoryBarrier.pNext = NULL;
  memoryBarrier.srcAccessMask = 0;
  memoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  memoryBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  memoryBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  memoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  memoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  memoryBarrier.subresourceRange = subresourceRange;
  memoryBarrier.image = image;

  vkCmdPipelineBarrier(postPresentCmdBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                       VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, NULL, 0, NULL,
                       1, &memoryBarrier);

  if (vkEndCommandBuffer(postPresentCmdBuffer) != VK_SUCCESS) {
    std::cout << "Unable to end command buffer" << std::endl;
    return false;
  }

  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.pNext = NULL;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &postPresentCmdBuffer;
  submitInfo.waitSemaphoreCount = 0;
  submitInfo.pWaitSemaphores = NULL;
  submitInfo.signalSemaphoreCount = 0;
  submitInfo.pSignalSemaphores = NULL;

  if (vkQueueSubmit(queue, 1, &submitInfo, NULL) != VK_SUCCESS) {
    std::cout << "Unable to submit queues" << std::endl;
    return false;
  }

  return true;
}
}
}
}
