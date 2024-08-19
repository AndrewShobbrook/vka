#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

typedef struct vka_PipelineLayoutCreateInfo {
} vka_PipelineLayoutCreateInfo;

/* Creates a relatively default pipeline layout. This system needs heavy improvement. Assume this will change */
VkPipelineLayout vka_CreatePipelineLayout(vka_PipelineLayoutCreateInfo *layout_create_info, VkDevice device);

/* Creates a default ish graphics pipeline using supplied shaders, layout and render_pass. This system needs heavy improvement. Assume this will change */
VkPipeline vka_CreateGraphicsPipeline(VkDevice device, VkShaderModule vert, VkShaderModule frag, VkPipelineLayout layout, VkRenderPass render_pass);
