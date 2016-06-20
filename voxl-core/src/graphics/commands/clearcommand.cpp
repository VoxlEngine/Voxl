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
  vk::VkContext *vkcontext = dynamic_cast<vk::VkContext *>(context);
  uint32_t imageCount =
      static_cast<uint32_t>(vkcontext->graphicsCmdBuffers.size());

  std::vector<VkImage> swapchainImages(imageCount);
  if (vkGetSwapchainImagesKHR(vkcontext->GetDevice(), vkcontext->GetSwapchain(),
                              &imageCount, &swapchainImages[0]) != VK_SUCCESS) {
    std::cout << "Unable to get swapchain images" << std::endl;
    return false;
  }

  VkCommandBufferBeginInfo beginInfo;
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.pNext = NULL;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  beginInfo.pInheritanceInfo = NULL;

  VkClearColorValue clearColor = {{color.x, color.y, color.z, color.w}};

  VkImageSubresourceRange subresourceRange;
  subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  subresourceRange.baseMipLevel = 0;
  subresourceRange.levelCount = 1;
  subresourceRange.baseArrayLayer = 0;
  subresourceRange.layerCount = 1;

  for (uint32_t i = 0; i < imageCount; i++) {
    // Begin command buffer
    vkBeginCommandBuffer(vkcontext->graphicsCmdBuffers[i], &beginInfo);

    // Set the image layout
    vk::SetImageLayout(vkcontext->graphicsCmdBuffers[i], swapchainImages[i],
                       subresourceRange, VK_IMAGE_LAYOUT_UNDEFINED,
                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    // Clear the image
    vkCmdClearColorImage(vkcontext->graphicsCmdBuffers[i], swapchainImages[i],
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1,
                         &subresourceRange);

    // Set the image layout back to undefined
    vk::SetImageLayout(vkcontext->graphicsCmdBuffers[i], swapchainImages[i],
                       subresourceRange, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                       VK_IMAGE_LAYOUT_UNDEFINED);

    // End command buffer
    if (vkEndCommandBuffer(vkcontext->graphicsCmdBuffers[i]) != VK_SUCCESS) {
      std::cout << "Unable to end command buffer" << std::endl;
      return false;
    }
  }
#elif defined(VOXL_API_GL)
  glClearColor(color.x, color.y, color.z, color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  return true;
#endif
}
}
}
}
