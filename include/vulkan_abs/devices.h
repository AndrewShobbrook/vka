#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <stdint.h>

VkPhysicalDevice getPhysicalDevice(VkInstance instance);

VkDevice createLogicalDevice(VkPhysicalDevice physical_device, VkInstance instance, uint32_t *queue_family);
