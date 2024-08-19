#include <vulkan/vulkan.h>

/* Basic RenderPassCreateInfo, only specify the color attachment format and depth attachment format */
typedef struct vka_RenderPassCreateInfo {
  VkFormat color_attachment_format;
  VkFormat depth_attachment_format;
} vka_RenderPassCreateInfo;

/* Creates a simple render pass based on the info passed to it. This system needs heavy improvement. Assume it will change */
VkRenderPass vka_CreateRenderPass(vka_RenderPassCreateInfo *render_pass_info, VkDevice device);