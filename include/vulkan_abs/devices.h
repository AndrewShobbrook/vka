#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <stdint.h>

/* Bits for configuration of preferred queues */
typedef enum vka_QueueConfiguration {
  vka_PREFER_PRESENT_GRAPHICS_QUEUE = 1,
  vka_COMPUTE_GRAPHICS_QUEUE = 1<<1,
  vka_COMPUTE_ONLY_QUEUE = 1<<2,
  vka_GRAPHICS_ONLY_QUEUE = 1<<3,
  vka_TRANSFER_ONLY_QUEUE = 1<<4,
  vka_PRESENT_QUEUE = 1<<5 // Not to be used in configuration. Used to pass back
} vka_QueueConfiguration;

/* Specifies whether you want integrated or dedicated graphics */
typedef enum vka_DeviceConfiguration {
  vka_PREFER_INTEGRATED = 1,
  vka_PREFER_DEDICATED = 1<<1,
} vka_DeviceConfiguration;

/* Return for every queue family, it's literally just a linked list. Traverse it.*/
typedef struct vka_QueueReturn {
  uint32_t queue_configuration;
  VkQueueFlags vk_flag_bits;
  uint32_t queue_family;
  uint32_t num_queues;
  struct vka_QueueReturn *p_next;
} vka_QueueReturn;

/* Returns the physical device and it's logical device and queues. Just traverse the queue return :skull: */
typedef struct vka_DeviceReturn {
  VkPhysicalDevice physical_device;
  VkDevice device;
  vka_QueueReturn* queue_return_start;
} vka_DeviceReturn;

/* Struct to specify all the parameters of the device and queue selection processes */
typedef struct {
  uint32_t requ_ext_count;
  char ** required_extensions;
  uint32_t pref_ext_count;
  char ** preferred_extensions;
  uint32_t queue_config;
  uint32_t device_config;
} vka_DeviceCreateInfo;

/* Creates a device based on the provided create_info and surface (Used for checking presentation capability) */
vka_DeviceReturn vka_CreateDevices(vka_DeviceCreateInfo *device_create_info, VkInstance instance, VkSurfaceKHR surface);
