#include <assert.h>
#include <stdio.h>
#include <vulkan/vulkan_core.h>
#include <vulkan_abs/pipelines.h>
#include <vulkan/vulkan.h>

VkPipelineLayout vka_CreatePipelineLayout(vka_PipelineLayoutCreateInfo *layout_create_info, VkDevice device) {
  VkPipelineLayoutCreateInfo create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .setLayoutCount = 0,
    .pushConstantRangeCount = 0,
  };

  VkPipelineLayout pipeline_layout;
  assert(vkCreatePipelineLayout(device, &create_info, NULL, &pipeline_layout) == VK_SUCCESS);
  return pipeline_layout;
}

typedef struct Vertex {
  float x;
  float y;
  float z;
  float a;
} Vertex;

VkPipeline vka_CreateGraphicsPipeline(VkDevice device, VkShaderModule vert, VkShaderModule frag, VkPipelineLayout layout, VkRenderPass render_pass) {
  VkPipelineShaderStageCreateInfo vert_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .stage = VK_SHADER_STAGE_VERTEX_BIT,
    .module = vert,
    .pName = "main",
    .pSpecializationInfo = NULL,
  };
  VkPipelineShaderStageCreateInfo frag_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
    .module = frag,
    .pName = "main",
    .pSpecializationInfo = NULL,
  };

  const VkPipelineShaderStageCreateInfo shaders[2] = {vert_create_info, frag_create_info};

  VkVertexInputBindingDescription binding_description = {
    .binding = 0,
    .stride = sizeof(Vertex),
    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
  };

  VkVertexInputAttributeDescription attribute_description = {
    .location = 0,
    .binding = 0,
    .format = VK_FORMAT_R32G32B32A32_SFLOAT,
    .offset = 0,
  };

  VkPipelineVertexInputStateCreateInfo vertex_input = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .vertexBindingDescriptionCount = 1,
    .pVertexBindingDescriptions = &binding_description,
    .vertexAttributeDescriptionCount = 1,
    .pVertexAttributeDescriptions = &attribute_description,
  };

  VkPipelineInputAssemblyStateCreateInfo assembly_state = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    .primitiveRestartEnable = VK_FALSE,
  };

  VkPipelineTessellationStateCreateInfo tesselation_state = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .patchControlPoints = 1,
  };

  VkPipelineViewportStateCreateInfo viewport_state = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .scissorCount = 1,
    .viewportCount = 1,
    .pScissors = NULL,
    .pViewports = NULL,
  };

  VkPipelineRasterizationStateCreateInfo rasterisation_state = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .depthClampEnable = VK_FALSE,
    .rasterizerDiscardEnable = VK_FALSE,
    .polygonMode = VK_POLYGON_MODE_FILL,
    .cullMode = VK_CULL_MODE_NONE,
    .frontFace = VK_FRONT_FACE_CLOCKWISE,
    .depthBiasEnable = VK_FALSE,
    .depthBiasConstantFactor = 0,
    .depthBiasClamp = 0,
    .depthBiasSlopeFactor = 0,
    .lineWidth = 1,
  };


  VkPipelineMultisampleStateCreateInfo multisample_state = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    .sampleShadingEnable = VK_FALSE,
    .pSampleMask = NULL,
    .alphaToOneEnable = VK_FALSE,
    .alphaToCoverageEnable = VK_FALSE,
    .minSampleShading = 0,
  };

  VkPipelineColorBlendAttachmentState color_blend_attachment = {
    .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
    .blendEnable = VK_FALSE,
  };

  VkPipelineColorBlendStateCreateInfo color_blend_state = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .logicOpEnable = VK_FALSE,
    .logicOp = VK_LOGIC_OP_AND,
    .attachmentCount = 1,
    .pAttachments = &color_blend_attachment,
    .blendConstants = {0},
  };

  VkDynamicState dynamic_states[] = {
    VK_DYNAMIC_STATE_SCISSOR,
    VK_DYNAMIC_STATE_VIEWPORT,
  };

  VkPipelineDynamicStateCreateInfo dynamic_state = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .dynamicStateCount = 2,
    .pDynamicStates = dynamic_states,
  };

  VkPipelineDepthStencilStateCreateInfo depth_state = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
    .depthTestEnable = VK_FALSE,
    .depthWriteEnable = VK_FALSE,
    .stencilTestEnable = VK_FALSE,
    .depthBoundsTestEnable = VK_FALSE,
  };


  VkGraphicsPipelineCreateInfo pipeline_create_info = {
    .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .stageCount = 2,
    .pStages = shaders,
    .pVertexInputState = &vertex_input,
    .pInputAssemblyState = &assembly_state,
    .pTessellationState = &tesselation_state,
    .pViewportState = &viewport_state,
    .pRasterizationState = &rasterisation_state,
    .pMultisampleState = &multisample_state,
    .pDepthStencilState = NULL,
    .pColorBlendState = &color_blend_state,
    .pDynamicState = &dynamic_state,
    .layout = layout,
    .renderPass = render_pass,
    .subpass = 0,
    .basePipelineHandle = VK_NULL_HANDLE,
    .basePipelineIndex = -1,
  };

  VkPipeline pipeline;

  #ifndef NDEBUG
  printf("Creating graphics pipeline\n");
  #endif /* ifndef NDEBUG */

  vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_create_info, NULL, &pipeline);

  #ifndef NDEBUG
  printf("Created pipeline\n");
  #endif /* ifndef NDEBUG */
  
  return pipeline;
}
