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

#ifndef VOXL_GRAPHICS_VK_CONTEXT_H_
#define VOXL_GRAPHICS_VK_CONTEXT_H_

#define VULKAN_VERSION_MAJOR 1
#define VULKAN_VERSION_MINOR 0
#define VULKAN_VERSION_PATCH 8
#include <vulkan/vulkan.h>

#include "voxl.hpp"
#include "types.hpp"
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

				inline GLFWwindow *GetWindow() { return window; };

				inline VkDevice GetDevice() { return dev; };
				inline VkInstance GetInstance() { return instance; };
				inline VkSwapchainKHR GetSwapchain() { return swapchain; };

				VkQueue queue;
				uint32 queueIndex;

				uint32 currentImage;

				std::vector<VkImage> swapchainImages;
				std::vector<VkImageView> swapchainImageViews;

				VkCommandPool cmdPool;

				std::vector<VkCommandBuffer> drawCmdBuffers;

			private:
				GLFWwindow *window;

				bool CreateInstance(const char* name);
				bool GetPhysicalDevice();
				bool CreateWindowSurface(int width, int height, const char* title);
				bool GetQueueFamilies();
				bool CreateDevice();
#ifdef VOXL_DEBUG
				bool SetupValidation();
#endif
				bool CreateSemaphores();
				bool CreateSwapchain(int width, int height);
				bool CreateCommandPool();
				bool CreateCommandBuffers();
				void BeginDrawBuffer(VkCommandBuffer cmdBuffer, VkImage image);
				void EndDrawBuffer(VkCommandBuffer cmdBuffer, VkImage image);

#ifdef VOXL_DEBUG
				PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = VK_NULL_HANDLE;
				PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT = VK_NULL_HANDLE;
				PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT = VK_NULL_HANDLE;

				VkDebugReportCallbackEXT msgCallback;
#endif

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