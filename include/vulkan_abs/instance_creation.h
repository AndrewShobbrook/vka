#include <vulkan/vulkan.h>

/* Struct that defines parameters for instance creation */
typedef struct {
  uint32_t requ_extension_count;
  char **required_extensions;
  uint32_t pref_extension_count;
  char **pref_extensions;
  uint32_t required_instance_version;
  uint32_t pref_instance_version;
} vka_InstanceCreateInfo;

/* Creates an instance based on the provided instance_create_info. Directly returns the VkResult from the instance creation call instead of handling itself */
VkResult vka_createInstance(vka_InstanceCreateInfo *instance_create_info, VkInstance *instance);
