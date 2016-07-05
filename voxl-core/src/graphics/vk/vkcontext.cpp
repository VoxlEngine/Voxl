/*
* Voxl Game Engine
* Copyright 2016 Sam H.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <cstring>
#include <iostream>
#include <vector>

#include "graphics/vk/util.hpp"
#include "graphics/vk/vkcontext.hpp"

namespace voxl {
	namespace graphics {
		namespace vk {
#ifdef VOXL_DEBUG
			VkBool32 MessageCallback(VkDebugReportFlagsEXT flags,
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

				std::cout << prefix.c_str() << "at " << location << " [" << pLayerPrefix << "] Code "
					<< msgCode << " : " << pMsg << std::endl;

				return VK_FALSE;
			}
#endif

			// TODO: Resizing
			bool VkContext::Init(Config config) {
				if (!glfwInit()) {
					std::cout << "Unable to initialize GLFW" << std::endl;
					return false;
				}

				if (!CreateInstance(config.windowTitle)) {
					std::cout << "Unable to create Vulkan instance" << std::endl;
					return false;
				}
				else {
					std::cout << "Created Vulkan instance" << std::endl;
				}

				if (!GetPhysicalDevice()) {
					std::cout << "Unable to get Vulkan physical device" << std::endl;
					return false;
				}
				else {
					std::cout << "Acquired Vulkan physical device" << std::endl;
				}

				if (!CreateWindowSurface(config.windowWidth, config.windowHeight, config.windowTitle)) {
					std::cout << "Unable to create window surfcace for Vulkan" << std::endl;
					return false;
				}
				else {
					std::cout << "Created Vulkan window surface" << std::endl;
				}

				if (!GetQueueFamilies()) {
					std::cout << "Unable to get Vulkan queue families" << std::endl;
					return false;
				}
				else {
					std::cout << "Acquired queue families" << std::endl;
				}

				if (!CreateDevice()) {
					std::cout << "Unable to create Vulkan device" << std::endl;
					return false;
				}
				else {
					std::cout << "Created Vulkan device" << std::endl;
				}

#ifdef VOXL_DEBUG
				if (!SetupValidation()) {
					std::cout << "Unable to setup Vulkan validation" << std::endl;
					return false;
				}
				else {
					std::cout << "Setup Vulkan validation" << std::endl;
				}
#endif

				if (!CreateSemaphores()) {
					std::cout << "Unable to create Vulkan semaphores" << std::endl;
					return false;
				}
				else {
					std::cout << "Created Vulkan semaphores" << std::endl;
				}

				if (!CreateSwapchain(config.windowWidth, config.windowHeight)) {
					std::cout << "Unable to create Vulkan swapchain" << std::endl;
					return false;
				}
				else {
					std::cout << "Created Vulkan swapchain" << std::endl;
				}

				if (!CreateCommandPool()) {
					std::cout << "Unable to create command pool" << std::endl;
					return false;
				}
				else {
					std::cout << "Created Vulkan command pool" << std::endl;
				}

				if (!CreateCommandBuffers()) {
					std::cout << "Unable to create command buffers" << std::endl;
					return false;
				}
				else {
					std::cout << "Created Vulkan command buffers" << std::endl;
				}

				std::cout << "Successfully initialized Vulkan" << std::endl;

				return true;
			}

			bool VkContext::CreateInstance(const char* name) {
				VkApplicationInfo appInfo = {};
				appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
				appInfo.pNext = nullptr;
				appInfo.pApplicationName = name;
				appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
				appInfo.pEngineName = "Voxl Engine";
				appInfo.engineVersion = VK_MAKE_VERSION(VOXL_VERSION_MAJOR, VOXL_VERSION_MINOR, VOXL_VERSION_PATCH);
				appInfo.apiVersion = VK_API_VERSION_1_0;

				std::vector<const char *> instanceExtensions;
				std::vector<const char *> instanceLayers;

#ifdef VOXL_DEBUG
				// Add debug report extension
				instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

				// Add validation layers
				instanceLayers.push_back("VK_LAYER_LUNARG_standard_validation");
#endif

				// Add GLFW extensions
				uint32 glfwExtensionCount = 0;
				const char **glfwExtensions =
					glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
				for (int i = 0; i < glfwExtensionCount; i++) {
					instanceExtensions.push_back(glfwExtensions[i]);
				}

				VkInstanceCreateInfo instanceInfo = {};
				instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				instanceInfo.pNext = nullptr;
				instanceInfo.flags = 0;
				instanceInfo.pApplicationInfo = &appInfo;
				instanceInfo.enabledExtensionCount = (uint32)instanceExtensions.size();
				instanceInfo.ppEnabledExtensionNames = instanceExtensions.data();
				instanceInfo.enabledLayerCount = (uint32)instanceLayers.size();
				instanceInfo.ppEnabledLayerNames = instanceLayers.data();

				CheckVkResult(vkCreateInstance(&instanceInfo, nullptr, &instance));

				return true;
			}

			// TODO: Rewrite to find first device supporting all required extensions
			bool VkContext::GetPhysicalDevice() {
				uint32 deviceCount = 1;

				CheckVkResult(vkEnumeratePhysicalDevices(instance, &deviceCount, &physDev));

				if (deviceCount == 0) {
					std::cout << "No Vulkan GPU found" << std::endl;
					return false;
				}

				return true;
			}

			bool VkContext::CreateWindowSurface(int width, int height, const char* title) {
				// Create window
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
				glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
				window = glfwCreateWindow(width, height, title, nullptr, nullptr);

				if (window == nullptr) {
					std::cout << "Unable to create GLFW window" << std::endl;
					return false;
				}

				// Create surface
				VkResult res = glfwCreateWindowSurface(instance, window, nullptr, &surf);
				if (res != VK_SUCCESS) {
					std::cout << "Unable to create window Vulkan surface: error " << res << std::endl;
					return false;
				}

				return true;
			}

			bool VkContext::GetQueueFamilies() {
				uint32 familyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(physDev, &familyCount, nullptr);

				if (familyCount == 0) {
					std::cout << "Vulkan GPU does not support any queue family" << std::endl;
					return false;
				}

				std::vector<VkQueueFamilyProperties> families(familyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(physDev, &familyCount, families.data());

				// Get a queue with support for both graphics and present
				uint32 graphicsQueueIndex = UINT32_MAX;
				uint32 presentQueueIndex = UINT32_MAX;
				for (uint32 i = 0; i < familyCount; i++) {
					VkBool32 presentSupport = VK_FALSE;
					vkGetPhysicalDeviceSurfaceSupportKHR(physDev, i, surf, &presentSupport);

					if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						if (graphicsQueueIndex == UINT32_MAX) {
							graphicsQueueIndex = i;
						}
						if (presentSupport) {
							graphicsQueueIndex = i;
							presentQueueIndex = i;
							break;
						}
					}
				}

				if (graphicsQueueIndex == UINT32_MAX || presentQueueIndex == UINT32_MAX) {
					std::cout
						<< "Present or graphics queue not found" << std::endl;
					return false;
				}

				if (graphicsQueueIndex != presentQueueIndex) {
					std::cout << "Unable to find queue which supports both present and graphics" << std::endl;
					return false;
				}

				queueIndex = graphicsQueueIndex;

				return true;
			}

			bool VkContext::CreateDevice() {
				float queuePriority = 1.0f;

				VkDeviceQueueCreateInfo queueCreateInfo = {};

				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueIndex;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &queuePriority;

				std::vector<const char*> deviceExtensions;
				std::vector<const char*> deviceLayers;

#ifdef VOXL_DEBUG
				// Add validation layers
				deviceLayers.push_back("VK_LAYER_LUNARG_standard_validation");
#endif

				deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

				VkDeviceCreateInfo deviceCreateInfo = {};
				deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
				deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
				deviceCreateInfo.queueCreateInfoCount = 1;
				deviceCreateInfo.enabledExtensionCount = (uint32)deviceExtensions.size();
				deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
				deviceCreateInfo.enabledLayerCount = (uint32)deviceLayers.size();
				deviceCreateInfo.ppEnabledLayerNames = deviceLayers.data();

				CheckVkResult(vkCreateDevice(physDev, &deviceCreateInfo, nullptr, &dev));

				// Get queue
				vkGetDeviceQueue(dev, queueIndex, 0, &queue);

				return true;
			}

#ifdef VOXL_DEBUG
			bool VkContext::SetupValidation() {
				// Set debug validation callback
				vkCreateDebugReportCallbackEXT =
					(PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(
						instance, "vkCreateDebugReportCallbackEXT");
				vkDestroyDebugReportCallbackEXT =
					(PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(
						instance, "vkDestroyDebugReportCallbackEXT");
				vkDebugReportMessageEXT = (PFN_vkDebugReportMessageEXT)vkGetInstanceProcAddr(
					instance, "vkDebugReportMessageEXT");

				VkDebugReportCallbackCreateInfoEXT dbgCreateInfo = {};
				dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
				dbgCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)MessageCallback;
				dbgCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
					VK_DEBUG_REPORT_WARNING_BIT_EXT |
					VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;

				CheckVkResult(vkCreateDebugReportCallbackEXT(instance, &dbgCreateInfo, nullptr, &msgCallback));

				return true;
			}
#endif

			bool VkContext::CreateSemaphores() {
				VkSemaphoreCreateInfo semaphoreCreateInfo = {};
				semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
				semaphoreCreateInfo.pNext = nullptr;

				CheckVkResult(vkCreateSemaphore(dev, &semaphoreCreateInfo, nullptr, &acquireCompleteSemaphore));
				std::cout << "Acquire complete: " << acquireCompleteSemaphore << std::endl;
				CheckVkResult(vkCreateSemaphore(dev, &semaphoreCreateInfo, nullptr, &renderCompleteSemaphore));
				std::cout << "Render complete: " << renderCompleteSemaphore << std::endl;
				return true;
			}

			bool VkContext::CreateSwapchain(int width, int height) {
				// Get surface capabilities
				VkSurfaceCapabilitiesKHR surfaceCapabilities;
				CheckVkResult(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDev, surf, &surfaceCapabilities));

				// Get surface formats
				uint32 formatCount = 0;
				CheckVkResult(vkGetPhysicalDeviceSurfaceFormatsKHR(physDev, surf, &formatCount, nullptr));

				std::vector<VkSurfaceFormatKHR> formats(formatCount);
				CheckVkResult(vkGetPhysicalDeviceSurfaceFormatsKHR(physDev, surf, &formatCount, formats.data()));

				// Get present modes
				uint32 presentModeCount;
				CheckVkResult(vkGetPhysicalDeviceSurfacePresentModesKHR(physDev, surf, &presentModeCount, nullptr));

				std::vector<VkPresentModeKHR> presentModes(presentModeCount);
				CheckVkResult(vkGetPhysicalDeviceSurfacePresentModesKHR(physDev, surf, &presentModeCount, presentModes.data()));

				// Select number of swapchain images
				uint32 imageCount = surfaceCapabilities.minImageCount + 1;
				if (surfaceCapabilities.maxImageCount != 0 && imageCount > surfaceCapabilities.maxImageCount) {
					imageCount = surfaceCapabilities.maxImageCount;
				}

				// Select format
				VkSurfaceFormatKHR format;

				if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED) {
					format.format = VK_FORMAT_R8G8B8A8_UNORM;
					format.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
				}
				else {
					for (uint32 i = 0; i < formats.size(); i++)
					{
						if (formats[i].format == VK_FORMAT_R8G8B8A8_UNORM) {
							format = formats[i];
						}
					}

					format = formats[0];
				}

				// Select extent
				VkExtent2D extent;

				if (surfaceCapabilities.currentExtent.width == -1) {
					extent.width = min(max(width, surfaceCapabilities.minImageExtent.width), surfaceCapabilities.maxImageExtent.width);
					extent.height = min(max(height, surfaceCapabilities.minImageExtent.height), surfaceCapabilities.maxImageExtent.height);
				}
				else {
					extent = surfaceCapabilities.currentExtent;
				}

				// Select present mode (FIFO by default)
				VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

				// Choose mailbox if possible
				for (uint32 i = 0; i < presentModes.size(); i++)
				{
					if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
						presentMode = presentModes[i];
					}
				}

				// Select transformation
				VkSurfaceTransformFlagBitsKHR transform;
				if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
					transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
				}
				else {
					transform = surfaceCapabilities.currentTransform;
				}

				// Check if image transfer destination is supported
				if (!(surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)) {
					std::cout << "Image transfer destination not supported" << std::endl;
					return false;
				}

				// Create swapchian
				VkSwapchainCreateInfoKHR swapchainInfo = {};
				swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
				swapchainInfo.surface = surf;
				swapchainInfo.minImageCount = imageCount;
				swapchainInfo.imageFormat = format.format;
				swapchainInfo.imageColorSpace = format.colorSpace;
				swapchainInfo.imageExtent = extent;
				swapchainInfo.imageArrayLayers = 1;
				swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
				swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				swapchainInfo.queueFamilyIndexCount = 0;
				swapchainInfo.pQueueFamilyIndices = nullptr;
				swapchainInfo.preTransform = transform;
				swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
				swapchainInfo.presentMode = presentMode;
				swapchainInfo.clipped = VK_TRUE;
				swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

				CheckVkResult(vkCreateSwapchainKHR(dev, &swapchainInfo, nullptr, &swapchain));

				// Get swapchain images
				uint32 swapchainImageCount = 0;
				CheckVkResult(vkGetSwapchainImagesKHR(dev, swapchain, &swapchainImageCount, nullptr));

				swapchainImages.resize(swapchainImageCount);
				CheckVkResult(vkGetSwapchainImagesKHR(dev, swapchain, &swapchainImageCount, swapchainImages.data()));

				// Create swapchain image views
				swapchainImageViews.resize(imageCount);
				for (uint32 i = 0; i < imageCount; i++) {
					VkImageViewCreateInfo imageViewCreateInfo = {};
					imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
					imageViewCreateInfo.image = swapchainImages[i];
					imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
					imageViewCreateInfo.format = format.format;
					imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
					imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
					imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
					imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
					imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
					imageViewCreateInfo.subresourceRange.levelCount = 1;
					imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
					imageViewCreateInfo.subresourceRange.layerCount = 1;

					CheckVkResult(vkCreateImageView(dev, &imageViewCreateInfo, nullptr, &swapchainImageViews[i]));
				}

				return true;
			}

			bool VkContext::CreateCommandPool() {
				// Create present command pool
				VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
				cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				cmdPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
				cmdPoolCreateInfo.queueFamilyIndex = queueIndex;

				CheckVkResult(vkCreateCommandPool(dev, &cmdPoolCreateInfo, nullptr, &cmdPool));

				return true;
			}

			bool VkContext::CreateCommandBuffers() {
				VkCommandBufferAllocateInfo allocateInfo = {};
				allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocateInfo.pNext = nullptr;
				allocateInfo.commandPool = cmdPool;
				allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

				// Allocate swapchain draw buffers
				allocateInfo.commandBufferCount = (uint32)swapchainImages.size();
				drawCmdBuffers.resize(swapchainImages.size());

				CheckVkResult(vkAllocateCommandBuffers(dev, &allocateInfo, drawCmdBuffers.data()));

				// Subresource range for clearing image
				VkImageSubresourceRange subresourceRange = {};
				subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				subresourceRange.baseMipLevel = 0;
				subresourceRange.levelCount = 1;
				subresourceRange.baseArrayLayer = 0;
				subresourceRange.layerCount = 1;

				VkClearColorValue clearColor = { {0.0f, 0.0f, 0.0f, 1.0f} };

				// Record the command buffer for every swap chain image
				for (uint32 i = 0; i < swapchainImages.size(); i++) {
					BeginDrawBuffer(drawCmdBuffers[i], swapchainImages[i]);

					vkCmdClearColorImage(drawCmdBuffers[i], swapchainImages[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &subresourceRange);

					EndDrawBuffer(drawCmdBuffers[i], swapchainImages[i]);
				}

				return true;
			}

			void VkContext::Destroy() {
				vkDeviceWaitIdle(dev);

				// Free command buffers
				if (drawCmdBuffers.size() > 0) {
					vkFreeCommandBuffers(dev, cmdPool, (uint32)drawCmdBuffers.size(),
						&drawCmdBuffers[0]);
				}

				// Destroy command pools
				vkDestroyCommandPool(dev, cmdPool, nullptr);

				// Destroy semaphores
				vkDestroySemaphore(dev, acquireCompleteSemaphore, nullptr);
				vkDestroySemaphore(dev, renderCompleteSemaphore, nullptr);

				// Destroy swapchain image views
				for (uint32 i = 0; i < swapchainImageViews.size(); i++) {
					vkDestroyImageView(dev, swapchainImageViews[i], nullptr);
				}

				// Destroy swapchain
				if (swapchain) {
					vkDestroySwapchainKHR(dev, swapchain, nullptr);
				}

				// Destroy surface
				if (surf) {
					vkDestroySurfaceKHR(instance, surf, nullptr);
				}

				// Destroy device
				if (dev) {
					vkDestroyDevice(dev, nullptr);
				}

#ifdef VOXL_DEBUG
				// Destroy debug report callback
				if (msgCallback != VK_NULL_HANDLE) {
					vkDestroyDebugReportCallbackEXT(instance, msgCallback, nullptr);
				}
#endif

				// Destroy instance
				if (instance) {
					vkDestroyInstance(instance, nullptr);
				}

				// Terminate GLFW
				glfwTerminate();
			}

			void VkContext::StartFrame() {
				// Acquire next image
				CheckVkResult(vkAcquireNextImageKHR(dev, swapchain, UINT64_MAX, acquireCompleteSemaphore, VK_NULL_HANDLE, &currentImage));
			}

			void VkContext::EndFrame() {
				// Submit current draw buffer
				VkSubmitInfo submitInfo = {};
				submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				submitInfo.pNext = nullptr;
				submitInfo.commandBufferCount = 1;
				submitInfo.pCommandBuffers = &drawCmdBuffers[currentImage];

				// Wait on image acquire
				submitInfo.waitSemaphoreCount = 1;
				submitInfo.pWaitSemaphores = &acquireCompleteSemaphore;

				// Only wait on transfer bit
				VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
				submitInfo.pWaitDstStageMask = &waitDstStageMask;

				// Signal render complete semaphore
				submitInfo.signalSemaphoreCount = 1;
				submitInfo.pSignalSemaphores = &renderCompleteSemaphore;

				if (vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
					std::cout << "Unable to submit command buffer" << std::endl;
				}

				VkPresentInfoKHR presentInfo = {};
				presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
				presentInfo.pNext = nullptr;
				presentInfo.waitSemaphoreCount = 1;
				presentInfo.pWaitSemaphores = &renderCompleteSemaphore;
				presentInfo.swapchainCount = 1;
				presentInfo.pSwapchains = &swapchain;
				presentInfo.pImageIndices = &currentImage;

				CheckVkResult(vkQueuePresentKHR(queue, &presentInfo));
			}

			void VkContext::BeginDrawBuffer(VkCommandBuffer cmdBuffer, VkImage image) {
				VkCommandBufferBeginInfo beginInfo = {};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

				VkImageSubresourceRange subresourceRange = {};
				subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				subresourceRange.baseMipLevel = 0;
				subresourceRange.levelCount = 1;
				subresourceRange.baseArrayLayer = 0;
				subresourceRange.layerCount = 1;

				VkImageMemoryBarrier memoryBarrier = {};
				memoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				memoryBarrier.srcAccessMask = 0;
				memoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				memoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				memoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				memoryBarrier.srcQueueFamilyIndex = queueIndex;
				memoryBarrier.dstQueueFamilyIndex = queueIndex;
				memoryBarrier.image = image;
				memoryBarrier.subresourceRange = subresourceRange;

				// Begin command buffer
				vkBeginCommandBuffer(cmdBuffer, &beginInfo);

				// Change image layout to transfer dst optimal
				vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &memoryBarrier);
			}

			void VkContext::EndDrawBuffer(VkCommandBuffer cmdBuffer, VkImage image) {
				VkImageSubresourceRange subresourceRange = {};
				subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				subresourceRange.baseMipLevel = 0;
				subresourceRange.levelCount = 1;
				subresourceRange.baseArrayLayer = 0;
				subresourceRange.layerCount = 1;

				VkImageMemoryBarrier memoryBarrier = {};
				memoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				memoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				memoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				memoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				memoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				memoryBarrier.srcQueueFamilyIndex = queueIndex;
				memoryBarrier.dstQueueFamilyIndex = queueIndex;
				memoryBarrier.image = image;
				memoryBarrier.subresourceRange = subresourceRange;
				
				// Change image layout to present
				vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &memoryBarrier);
			
				// End command buffer
				CheckVkResult(vkEndCommandBuffer(cmdBuffer));
			}
		}
	}
}