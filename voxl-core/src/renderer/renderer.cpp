#include <cstring>
#include <iostream>
#include <vector>

#include "renderer/renderer.hpp"

namespace voxl {
namespace renderer {
Renderer::Renderer(Config config) {
  VkResult result;

  if (!glfwInit()) {
    std::cout << "Unable to initialize GLFW" << std::endl;
  }

  if (!glfwVulkanSupported()) {
    std::cout << "GLFW does not support Vulkan" << std::endl;
  }

  uint32_t numExtensions = 0;
  const char **extensionNames =
      glfwGetRequiredInstanceExtensions(&numExtensions);

  VkApplicationInfo appInfo;
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pNext = NULL;
  // TODO: Use application name
  appInfo.pApplicationName = config.windowTitle;
  appInfo.pEngineName = ENGINE_NAME;
  appInfo.engineVersion = VK_MAKE_VERSION(
      VOXL_VERSION_MAJOR, VOXL_VERSION_MINOR, VOXL_VERSION_PATCH);
  appInfo.apiVersion = VK_MAKE_VERSION(
      VULKAN_VERSION_MAJOR, VULKAN_VERSION_MINOR, VULKAN_VERSION_PATCH);

  VkInstanceCreateInfo instanceInfo;
  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pNext = NULL;
  instanceInfo.flags = 0;
  instanceInfo.pApplicationInfo = &appInfo;
  instanceInfo.enabledLayerCount = 0;
  instanceInfo.ppEnabledLayerNames = NULL;
  instanceInfo.enabledExtensionCount = numExtensions;
  instanceInfo.ppEnabledExtensionNames = extensionNames;

  result = vkCreateInstance(&instanceInfo, NULL, &instance);
  if (result != VK_SUCCESS) {
    std::cout << "Unable to create Vulkan instance: Error code " << result
              << std::endl;
  }

  uint32_t numPhysicalDevices = 0;
  result = vkEnumeratePhysicalDevices(instance, &numPhysicalDevices, NULL);

  if (result != VK_SUCCESS) {
    std::cout << "Unable to query number of GPUs" << std::endl;
  }

  if (numPhysicalDevices == 0) {
    std::cout << "No compatilble Vulkan GPU" << std::endl;
  }

  std::vector<VkPhysicalDevice> physicalDevices(numPhysicalDevices);
  result = vkEnumeratePhysicalDevices(instance, &numPhysicalDevices,
                                      &physicalDevices[0]);
  if (result != VK_SUCCESS) {
    std::cout << "Unable to get list of GPUs" << std::endl;
  }

  VkPhysicalDeviceProperties deviceProperties;
  for (int i = 0; i < numPhysicalDevices; i++) {
    memset(&deviceProperties, 0, sizeof(deviceProperties));
    vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);
    std::cout << "Vulkan GPU " << i + 1 << ":" << std::endl;
    std::cout << "  Driver Version: "
              << (deviceProperties.driverVersion >> 22 & 0x3ff) << "."
              << (deviceProperties.driverVersion >> 12 & 0x3ff) << "."
              << (deviceProperties.driverVersion & 0xfff) << std::endl;
    std::cout << "  GPU Name: " << deviceProperties.deviceName << std::endl;
    std::cout << "  GPU Type: " << deviceProperties.deviceType << std::endl;
    std::cout << "  Vulkan API Version: "
              << (deviceProperties.apiVersion >> 22 & 0x3ff) << "."
              << (deviceProperties.apiVersion >> 12 & 0x3ff) << "."
              << (deviceProperties.apiVersion & 0xfff) << std::endl;
  }

  VkDeviceCreateInfo devCreateInfo;
  devCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  devCreateInfo.pNext = NULL;
  devCreateInfo.flags = 0;

  devCreateInfo.enabledLayerCount = 0;
  devCreateInfo.ppEnabledLayerNames = NULL;
  devCreateInfo.enabledExtensionCount = 0;
  devCreateInfo.ppEnabledExtensionNames = NULL;
  devCreateInfo.pEnabledFeatures = NULL;

  VkDeviceQueueCreateInfo devQueueInfo;
  devQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  devQueueInfo.pNext = NULL;
  devQueueInfo.flags = 0;
  devQueueInfo.queueFamilyIndex = 0;

  float queuePriorities[] = {1.0f};
  devQueueInfo.queueCount = 1;
  devQueueInfo.pQueuePriorities = queuePriorities;

  devCreateInfo.queueCreateInfoCount = 1;
  devCreateInfo.pQueueCreateInfos = &devQueueInfo;

  result = vkCreateDevice(physicalDevices[0], &devCreateInfo, NULL, &dev);
  if (result != VK_SUCCESS) {
    std::cout << "Unabled to create Vulkan device" << std::endl;
    std::cout << result;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  window = glfwCreateWindow(config.windowWidth, config.windowHeight,
                            config.windowTitle, NULL, NULL);

  result = glfwCreateWindowSurface(instance, window, NULL, &surf);
  if (result != VK_SUCCESS) {
    std::cout << "Unable to create window Vulkan surface" << std::endl;
  }
}
Renderer::~Renderer() { vkDestroyInstance(instance, NULL); }
}
}
