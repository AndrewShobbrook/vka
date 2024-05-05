#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan_abs/instance_creation.h>

VkResult vka_createInstance(vka_InstanceCreateInfo *instance_create_info,
                            VkInstance *instance) {
  // Getting instance version
  uint32_t instance_version;
  vkEnumerateInstanceVersion(&instance_version);

  // Enumerating the available instance extensions and storing them

  uint32_t count;
  vkEnumerateInstanceExtensionProperties(NULL, &count, NULL);
  VkExtensionProperties *instance_extensions =
      malloc(count * sizeof(VkExtensionProperties));
  printf("%p\n", instance_extensions);
  vkEnumerateInstanceExtensionProperties(NULL, &count, instance_extensions);

  // Storing found state in bool array

  bool *required_extensions =
      calloc(instance_create_info->requ_extension_count, 1);
  bool *preferred_extensions =
      calloc(instance_create_info->pref_extension_count, 1);

  for (int i = 0; i < count; i++) {
    printf("%s\n", instance_extensions[i].extensionName);
    for (int j = 0; j < instance_create_info->requ_extension_count; j++) {
      if (strcmp(instance_extensions[i].extensionName,
                 instance_create_info->required_extensions[j]) == 0) {
        required_extensions[j] = true;
        break;
      }
    }
    for (int j = 0; j < instance_create_info->pref_extension_count; j++) {
      if (strcmp(instance_extensions[i].extensionName,
                 instance_create_info->pref_extensions[j]) == 0) {
        preferred_extensions[j] = true;
        break;
      }
    }
  }

  free(instance_extensions);

  for (int i = 0; i < instance_create_info->requ_extension_count; i++) {
    if (required_extensions[i] != true) {
      return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
  }

  // Creating array to hold extensions

  const char **extensions;
  uint32_t found_extension_count = 0;
  if (instance_create_info->requ_extension_count +
          instance_create_info->pref_extension_count ==
      0)
    extensions = NULL;
  else {
    extensions = malloc((instance_create_info->requ_extension_count +
                         instance_create_info->pref_extension_count) *
                        sizeof(char **));
    for (int i = 0; i < instance_create_info->requ_extension_count; i++) {
      extensions[i] = instance_create_info->required_extensions[i];
    }
    for (int i = 0; i < instance_create_info->pref_extension_count; i++) {
      if (preferred_extensions[i] != true) {
        continue;
      } else {
        instance_create_info
            ->required_extensions[found_extension_count +
                                  instance_create_info->requ_extension_count] =
            instance_create_info->pref_extensions[i];
        found_extension_count += 1;
      }
    }
  }

  VkInstanceCreateInfo create_info = {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .flags = 0,
      .pNext = NULL,
      .pApplicationInfo = NULL,
      .enabledLayerCount = 0,
      .enabledExtensionCount =
          instance_create_info->requ_extension_count + found_extension_count,
      .ppEnabledExtensionNames = extensions,
  };

  return vkCreateInstance(&create_info, NULL, instance);
}
