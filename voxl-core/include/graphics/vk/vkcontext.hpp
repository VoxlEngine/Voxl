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
				uint32_t queueIndex;

				uint32_t currentImage;

				std::vector<VkImage> swapchainImages;
				std::vector<VkImageView> swapchainImageViews;

				VkCommandPool cmdPool;

				VkCommandBuffer setupCmdBuffer;
				VkCommandBuffer prePresentCmdBuffer;
				VkCommandBuffer postPresentCmdBuffer;

				std::vector<VkCommandBuffer> drawCmdBuffers;

			private:
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
				bool PrePresentBarrier(VkImage image);
				bool PostPresentBarrier(VkImage image);

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