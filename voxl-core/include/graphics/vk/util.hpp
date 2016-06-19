#ifndef VOXL_GRAPHICS_VK_UTIL_H_
#define VOXL_GRAPHICS_VK_UTIL_H_

#define VULKAN_VERSION_MAJOR 1
#define VULKAN_VERSION_MINOR 0
#define VULKAN_VERSION_PATCH 8
#include <vulkan/vulkan.h>

#include "voxl.hpp"

namespace voxl {
namespace graphics {
namespace vk {
void SetImageLayout(VkCommandBuffer buffer, VkImage image,
                    VkImageSubresourceRange subresourceRange,
                    VkImageLayout oldLayout, VkImageLayout newLayout);
}
}
}

#endif
