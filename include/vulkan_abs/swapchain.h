#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#include <stdint.h>

typedef struct vka_SwapchainReturn {
  VkSwapchainKHR swapchain;
  VkExtent2D extent;
  VkSurfaceFormatKHR format;
  VkImage *images;
  uint32_t count;
} vka_SwapchainReturn;


vka_SwapchainReturn vka_CreateSwapchain(VkPhysicalDevice phys_device, VkDevice device, GLFWwindow* window, VkSurfaceKHR surface, VkSwapchainKHR old_swapchain);

VkImageView *vka_CreateSwapchainImageViews(VkDevice device, vka_SwapchainReturn swap_return);
