#include <iostream>
#include <vector>

#include "graphics/commands/clearcommand.hpp"
#include "graphics/vk/util.hpp"
#include "graphics/vk/vkcontext.hpp"

namespace voxl {
	namespace graphics {
		namespace commands {
			ClearCommand::ClearCommand(math::Vec4 clearColor) : color(clearColor) {}
			bool ClearCommand::Execute(Context *context) {
#ifdef VOXL_API_VK
				/*vk::VkContext *vkcontext = static_cast<vk::VkContext *>(context);
				uint32_t imageCount = static_cast<uint32_t>(vkcontext->drawCmdBuffers.size());

				VkCommandBufferBeginInfo beginInfo;
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.pNext = nullptr;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
				beginInfo.pInheritanceInfo = nullptr;

				VkClearColorValue clearColor = { {color.x, color.y, color.z, color.w} };

				VkImageSubresourceRange subresourceRange;
				subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				subresourceRange.baseMipLevel = 0;
				subresourceRange.levelCount = 1;
				subresourceRange.baseArrayLayer = 0;
				subresourceRange.layerCount = 1;

				for (uint32_t i = 0; i < imageCount; i++) {
					// Begin command buffer
					vkBeginCommandBuffer(vkcontext->drawCmdBuffers[i], &beginInfo);

					// Clear the image
					vkCmdClearColorImage(vkcontext->drawCmdBuffers[i],
						vkcontext->swapchainImages[i],
						VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1,
						&subresourceRange);

					// End command buffer
					if (vkEndCommandBuffer(vkcontext->drawCmdBuffers[i]) != VK_SUCCESS) {
						std::cout << "Unable to end command buffer" << std::endl;
						return false;
					}
				}*/

				return true;
#elif defined(VOXL_API_GL)
				glClearColor(color.x, color.y, color.z, color.w);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				return true;
#endif
			}
		}
	}
}