#include <stdint.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <stdio.h>
#include <vulkan_abs/devices.h>

vka_DeviceReturn vka_CreateDevices(vka_DeviceCreateInfo *device_create_info, VkInstance instance, VkSurfaceKHR surface) {
  uint32_t device_count;
  vka_DeviceReturn device_return;
  vkEnumeratePhysicalDevices(instance, &device_count, NULL);
  VkPhysicalDevice *phys_devices = malloc(device_count * sizeof(phys_devices));
  vkEnumeratePhysicalDevices(instance, &device_count, phys_devices);

  uint32_t *device_scores = malloc(device_count * sizeof(uint32_t));

  VkPhysicalDevice final_device = NULL;

  for (int i = 0; i < device_count; i++) {
    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(phys_devices[i], &device_features);

    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(phys_devices[i], &device_properties);

    if ((device_create_info->device_config & vka_PREFER_DEDICATED) &&
      (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)) {
      final_device = phys_devices[i];
      goto physical_device_selected;
    } 

    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(phys_devices[i], NULL, &extension_count, NULL);
    VkExtensionProperties *extensions = malloc(extension_count * sizeof(VkExtensionProperties));
    vkEnumerateDeviceExtensionProperties(phys_devices[i], NULL, &extension_count, extensions);
    free(extensions);
  }


physical_device_selected:
  free(device_scores);
  free(phys_devices);

  device_return.physical_device = final_device;

  // Begin handling Device Creation

  VkDevice device;

  VkQueueFamilyProperties *queue_families;
  uint32_t queue_family_count;
  vkGetPhysicalDeviceQueueFamilyProperties(final_device, &queue_family_count, NULL);
  queue_families = malloc(queue_family_count * sizeof(VkQueueFamilyProperties));
  vkGetPhysicalDeviceQueueFamilyProperties(final_device, &queue_family_count, queue_families);

  for (int i = 0; i < queue_family_count; i++) {

  }
}
