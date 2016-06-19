#include "graphics/vk/util.hpp"

namespace voxl {
namespace graphics {
namespace vk {
void SetImageLayout(VkCommandBuffer buffer, VkImage image,
                    VkImageSubresourceRange subresourceRange,
                    VkImageLayout oldLayout, VkImageLayout newLayout) {
  VkImageMemoryBarrier memoryBarrier;
  memoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  memoryBarrier.pNext = NULL;
  memoryBarrier.oldLayout = oldLayout;
  memoryBarrier.newLayout = newLayout;
  memoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  memoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  memoryBarrier.subresourceRange = subresourceRange;

  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED) {
    memoryBarrier.srcAccessMask =
        VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
  }

  if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
    memoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  }

  if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
    memoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  }

  if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    memoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
  }

  if (newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    memoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  }

  if (newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
    memoryBarrier.srcAccessMask =
        memoryBarrier.srcAccessMask | VK_ACCESS_TRANSFER_READ_BIT;
    memoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  }

  if (newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
    memoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    memoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  }

  if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    memoryBarrier.dstAccessMask = memoryBarrier.dstAccessMask |
                                  VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  }

  if (newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    memoryBarrier.srcAccessMask =
        VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
    memoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  }

  vkCmdPipelineBarrier(buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                       VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, NULL, 0, NULL,
                       1, &memoryBarrier);
}
}
}
}
