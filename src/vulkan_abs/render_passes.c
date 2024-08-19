#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan_abs/render_passes.h>
#include <assert.h>

// For now only a single subpass is supported. Will change that once needed
VkRenderPass vka_CreateRenderPass(vka_RenderPassCreateInfo *render_pass_info, VkDevice device) {
  VkAttachmentReference color_attachment_reference = {
    .attachment = 0,
    .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  };

  VkAttachmentReference depth_attachment_reference = {
    .attachment = 1,
    .layout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
  };

  VkSubpassDescription subpass = {
    .flags = 0,
    .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
    .inputAttachmentCount = 0,
    .pInputAttachments = NULL,
    .colorAttachmentCount = 1,
    .pColorAttachments = &color_attachment_reference,
    .pResolveAttachments = NULL,
    .pDepthStencilAttachment = NULL,
    .preserveAttachmentCount = 0,
    .pPreserveAttachments = NULL,
  };

  VkAttachmentDescription depth_attachment = {
    .flags = 0,
    .format = render_pass_info->depth_attachment_format,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
    .finalLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
  };
  VkAttachmentDescription color_attachment = {
    .flags = 0,
    .format = render_pass_info->color_attachment_format,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  };
  VkAttachmentDescription attachment_descriptions[] = {color_attachment};

  VkRenderPassCreateInfo create_info = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .attachmentCount = 1,
    .pAttachments = attachment_descriptions,
    .subpassCount = 1,
    .pSubpasses = &subpass,
    .dependencyCount = 0,
    .pDependencies = NULL,
  };

  VkRenderPass render_pass;
  assert(vkCreateRenderPass(device, &create_info, NULL, &render_pass) == VK_SUCCESS);

  return render_pass;
}
