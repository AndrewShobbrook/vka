#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

struct vka_PipelineStateHolder {
  VkPipelineLayout pipeline_layout;
  VkPipeline pipeline;
};
