#include <stdint.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <stdio.h>

void handleQueues(VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice physical_device) {
  uint32_t count;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, NULL);
  VkQueueFamilyProperties *queue_families = malloc(count * sizeof(VkQueueFamilyProperties));

  uint32_t present_queue = -1;
  uint32_t graphics_queue = -1;
  uint32_t transfer_queue = -1;
  uint32_t compute_queue = -1;

  VkBool32 presentable_queue;
  for (int i = 0; i < count; i++) {
    vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &presentable_queue);

    if (presentable_queue == VK_TRUE) present_queue = i;

  }

}

VkPhysicalDevice getPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
  uint32_t count;
  vkEnumeratePhysicalDevices(instance, &count, NULL);
  if (count == 0) {
    fprintf(stderr, "Failed to find any physical devices");
    return 0;
  }
  VkPhysicalDevice *array = malloc(count * sizeof(VkPhysicalDevice));
  vkEnumeratePhysicalDevices(instance, &count, array);

  VkPhysicalDevice device;

  for (int i = 0; i < count; i++) {
    VkPhysicalDeviceProperties dev_properties;
    vkGetPhysicalDeviceProperties(array[i], &dev_properties);
    printf("Device name: %s", dev_properties.deviceName);
    device = array[i];
  }
  free(array);

  return device;
}

VkDevice createLogicalDevice(VkPhysicalDevice physical_device, VkInstance instance, uint32_t *queue_family) {

  uint32_t count;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, NULL);
  VkQueueFamilyProperties *queue_families = malloc(count * sizeof(VkQueueFamilyProperties));
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, queue_families);

  // Selection of queue families
  // For now, just select first queue family with general support
  
  uint32_t queue_family_index = 1000;
  for (int i = 0; i < count; i++) {
    if ((queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && 
      (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT)) {
      queue_family_index = i;
      break;
    }
  }
  if (queue_family_index == 1000) return 0;
  free(queue_families);

  float queue_priority = 1.0;

  VkDeviceQueueCreateInfo queue_create_info = {
    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .queueFamilyIndex = queue_family_index,
    .queueCount = 1,
    .pQueuePriorities = &queue_priority,
  };

  const char *device_extensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };

  VkDeviceCreateInfo creation_info = {
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .enabledLayerCount = 0,
    .queueCreateInfoCount = 1,
    .pQueueCreateInfos = &queue_create_info,
    .enabledExtensionCount = 1,
    .ppEnabledExtensionNames = device_extensions,
    .pEnabledFeatures = NULL,
  };

  VkDevice device;

  vkCreateDevice(physical_device, &creation_info, NULL, &device);

  *queue_family = queue_family_index;

  return device;
}
