#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <stdint.h>

typedef struct vka_SwapchainReturn {
  VkSwapchainKHR swapchain;
  VkExtent2D extent;
  VkSurfaceFormatKHR format;
  uint32_t count;
} vka_SwapchainReturn;


vka_SwapchainReturn vka_CreateSwapchain(VkPhysicalDevice phys_device, VkSurfaceKHR surface, VkSwapchainKHR old_swapchain);
