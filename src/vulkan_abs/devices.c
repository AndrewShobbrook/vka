#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan_abs/devices.h>

vka_DeviceReturn vka_CreateDevices(vka_DeviceCreateInfo *device_create_info,
                                   VkInstance instance, VkSurfaceKHR surface) {
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
        (device_properties.deviceType ==
         VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)) {
      final_device = phys_devices[i];
      goto physical_device_selected;
    }

    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(phys_devices[i], NULL,
                                         &extension_count, NULL);
    VkExtensionProperties *extensions =
        malloc(extension_count * sizeof(VkExtensionProperties));
    vkEnumerateDeviceExtensionProperties(phys_devices[i], NULL,
                                         &extension_count, extensions);
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
  vkGetPhysicalDeviceQueueFamilyProperties(final_device, &queue_family_count,
                                           NULL);
  queue_families = malloc(queue_family_count * sizeof(VkQueueFamilyProperties));
  vkGetPhysicalDeviceQueueFamilyProperties(final_device, &queue_family_count,
                                           queue_families);

  vka_QueueReturn *start_return = NULL;
  vka_QueueReturn *curr_return = NULL;

  int present_candidate = -1;
  int present_found = 0; // Set to 1 if present but not graphics, 2 if present and graphics
  #ifndef NDEBUG
  printf("queue_family_count: %d\n", queue_family_count);
  #endif
  for (int i = 0; i < queue_family_count; i++) {
    VkQueueFlags flags = queue_families[i].queueFlags;
    uint32_t count = queue_families[i].queueCount;
    VkBool32 present_supported;
    vkGetPhysicalDeviceSurfaceSupportKHR(final_device, i, surface,
                                         &present_supported);
    #ifndef NDEBUG
    printf("family: %d\tflags: %d\tcount: %d\n", i, flags, count);
    #endif

    if (present_supported == VK_TRUE && !present_found) {
      present_candidate = i;
      if (device_create_info->queue_config & vka_PREFER_PRESENT_GRAPHICS_QUEUE && flags & VK_QUEUE_GRAPHICS_BIT) {
        present_found = 2;
      }
    }

    // Transfer only queue check

    if (device_create_info->queue_config & vka_TRANSFER_ONLY_QUEUE) {
      if (!(flags & (VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT)) &&
          flags & VK_QUEUE_TRANSFER_BIT) {
        if (!start_return) {
          start_return = malloc(1 * sizeof(vka_QueueReturn));
          curr_return = start_return;
          start_return->p_next = NULL;
          start_return->num_queues = count;
          start_return->queue_family = i;
          start_return->vk_flag_bits = flags;
          start_return->queue_configuration = vka_TRANSFER_ONLY_QUEUE;
        } else {
          vka_QueueReturn *new_return = malloc(1 * sizeof(vka_QueueReturn));
          curr_return->p_next = new_return;
          new_return->p_next = NULL;
          new_return->num_queues = count;
          new_return->queue_family = i;
          new_return->vk_flag_bits = flags;
          new_return->queue_configuration = vka_TRANSFER_ONLY_QUEUE;
          curr_return = new_return;
        }
        continue;
      }
    }

    // Compute Graphics Queue

    if (device_create_info->queue_config & vka_COMPUTE_GRAPHICS_QUEUE) {
      if ((flags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)) ==
          (VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT)) {
        if (!start_return) {
          start_return = malloc(1 * sizeof(vka_QueueReturn));
          curr_return = start_return;
          start_return->p_next = NULL;
          start_return->num_queues = count;
          start_return->queue_family = i;
          start_return->vk_flag_bits = flags;
          start_return->queue_configuration = vka_COMPUTE_GRAPHICS_QUEUE;
        } else {
          vka_QueueReturn *new_return = malloc(1 * sizeof(vka_QueueReturn));
          curr_return->p_next = new_return;
          new_return->p_next = NULL;
          new_return->num_queues = count;
          new_return->queue_family = i;
          new_return->vk_flag_bits = flags;
          new_return->queue_configuration = vka_COMPUTE_GRAPHICS_QUEUE;
          curr_return = new_return;
        }
        continue;
      }
    }

    // Compute only queue

    if (device_create_info->queue_config & vka_COMPUTE_ONLY_QUEUE) {
      if ((flags & VK_QUEUE_COMPUTE_BIT) && !(flags & VK_QUEUE_GRAPHICS_BIT)) {
        if (!start_return) {
          start_return = malloc(1 * sizeof(vka_QueueReturn));
          curr_return = start_return;
          start_return->p_next = NULL;
          start_return->num_queues = count;
          start_return->queue_family = i;
          start_return->vk_flag_bits = flags;
          start_return->queue_configuration = vka_COMPUTE_ONLY_QUEUE;
        } else {
          vka_QueueReturn *new_return = malloc(1 * sizeof(vka_QueueReturn));
          curr_return->p_next = new_return;
          new_return->p_next = NULL;
          new_return->num_queues = count;
          new_return->queue_family = i;
          new_return->vk_flag_bits = flags;
          new_return->queue_configuration = vka_COMPUTE_ONLY_QUEUE;
          curr_return = new_return;
        }
        continue;
      }
    }

    // Graphics only queue

    if (device_create_info->queue_config & vka_GRAPHICS_ONLY_QUEUE) {
      if (!(flags & VK_QUEUE_COMPUTE_BIT) && (flags & VK_QUEUE_GRAPHICS_BIT)) {
        if (!start_return) {
          start_return = malloc(1 * sizeof(vka_QueueReturn));
          curr_return = start_return;
          start_return->p_next = NULL;
          start_return->num_queues = count;
          start_return->queue_family = i;
          start_return->vk_flag_bits = flags;
          start_return->queue_configuration = vka_GRAPHICS_ONLY_QUEUE;
        } else {
          vka_QueueReturn *new_return = malloc(1 * sizeof(vka_QueueReturn));
          curr_return->p_next = new_return;
          new_return->p_next = NULL;
          new_return->num_queues = count;
          new_return->queue_family = i;
          new_return->vk_flag_bits = flags;
          new_return->queue_configuration = vka_GRAPHICS_ONLY_QUEUE;
          curr_return = new_return;
        }
        continue;
      }
    }
  }

  vka_QueueReturn *next = start_return;
  int count = 0;
  while (next != NULL) {
    #ifndef NDEBUG
    printf("Index: %d\tCount: %d\tFamily: %d\n", count, next->num_queues, next->queue_family);
    #endif
    count ++;
    if (next->queue_family == present_candidate) next->queue_configuration = next->queue_configuration | vka_PRESENT_QUEUE;
    next = next->p_next;
  }

  // Begin Logical Device creation
  VkPhysicalDeviceFeatures features = {
  .tessellationShader = VK_TRUE,
  };
  const char *device_extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME, NULL};
  VkDeviceCreateInfo logical_device_create_info = {
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .flags = 0,
    .pNext = NULL,
    .pEnabledFeatures = &features,
    .enabledLayerCount = 0,
    .enabledExtensionCount = 1,
    .ppEnabledExtensionNames = device_extensions,
    .queueCreateInfoCount = count,
  };
  VkDeviceQueueCreateInfo *queue_create_infos = malloc(count * sizeof(VkDeviceQueueCreateInfo));

  next = start_return;
  for (int i = 0; i < count; i++) {
    queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_infos[i].pNext = NULL;
    queue_create_infos[i].flags = 0;
    float *queue_priorities = malloc(next->num_queues * sizeof(float));
    for (int j = 0; j< next->num_queues; j++) queue_priorities[j] = 1.0f;
    queue_create_infos[i].pQueuePriorities = queue_priorities;
    queue_create_infos[i].queueCount = next->num_queues;
    queue_create_infos[i].queueFamilyIndex = next->queue_family;
    next = next->p_next;
  }

  logical_device_create_info.pQueueCreateInfos = queue_create_infos;
  VkResult result = vkCreateDevice(final_device, &logical_device_create_info, NULL, &device_return.device);
  device_return.queue_return_start = start_return;
  return device_return;
}
