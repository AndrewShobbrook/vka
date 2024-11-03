#pragma once
#include <vulkan/vulkan.h>
#include <stdint.h>

/* Creates a standard Command Pool attached to the specified queue family */
VkCommandPool createCommandPool(VkDevice device, uint32_t queue_family);
