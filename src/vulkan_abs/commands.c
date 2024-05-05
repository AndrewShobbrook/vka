#include <stdint.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

VkCommandPool createCommandPool(VkDevice device, uint32_t queue_family) {
  VkCommandPoolCreateInfo create_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .flags = 0,
    .pNext = NULL,
    .queueFamilyIndex = queue_family,
  };

  VkCommandPool command_pool;

  vkCreateCommandPool(device, &create_info, NULL, &command_pool);

  return command_pool;
}
