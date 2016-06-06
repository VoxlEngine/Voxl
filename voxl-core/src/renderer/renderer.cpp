#include <cstring>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

#include "voxl.hpp"

#include "renderer/renderer.hpp"

namespace voxl {
namespace renderer {
Renderer::Renderer() {
  VkResult result;

  VkApplicationInfo appInfo;
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pNext = NULL;
  // TODO: Use application name
  appInfo.pApplicationName = NULL;
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
  instanceInfo.enabledExtensionCount = 0;
  instanceInfo.ppEnabledExtensionNames = NULL;

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
}
Renderer::~Renderer() { vkDestroyInstance(instance, NULL); }
}
}
