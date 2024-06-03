#include "vulkan_abs/swapchain.h"
#include <vulkan/vulkan_core.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

vka_SwapchainReturn vka_CreateSwapchain(VkPhysicalDevice phys_device, VkSurfaceKHR surface, VkSwapchainKHR old_swapchain) {
  vka_SwapchainReturn swap_return = {0};
  uint32_t count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(phys_device, surface, &count, NULL);
  VkSurfaceFormatKHR *surface_formats = malloc(count * sizeof(VkSurfaceFormatKHR));
  vkGetPhysicalDeviceSurfaceFormatsKHR(phys_device, surface, &count, surface_formats);

  uint32_t index_found = -1;
  uint32_t index_in_array = -1;
  VkFormat desired_formats[] = {VK_FORMAT_R8G8B8_SRGB};


  // Selecting surface format
  for (int i = 0; i < count; i++) {
#ifndef NDEBUG
    printf("Format %d: %d, %d\n", i, surface_formats[i].format, surface_formats[i].colorSpace);
#endif
    for (int j = 0; j < sizeof(desired_formats)/sizeof(desired_formats[0]); j++) {
      if (surface_formats[i].format == desired_formats[j] && surface_formats[i].colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
        if (j < index_found) {
          index_found = j;
          index_in_array = i;
        }
      }
    }
  }

  count = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(phys_device, surface, &count, NULL);
  VkPresentModeKHR *present_modes = malloc(count * sizeof(count));

  VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;

  for (int i = 0; i < count; i++) {
    if (present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
  }
  
  VkSurfaceCapabilitiesKHR surface_capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phys_device, surface, &surface_capabilities);

  assert(surface_capabilities.currentExtent.width != 0xffffffff);

  VkSwapchainCreateInfoKHR create_info = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext = NULL,
    .flags = 0,
    .surface = surface,
    .minImageCount = surface_capabilities.minImageCount,
    .imageFormat = surface_formats[index_in_array].format,
    .imageColorSpace = surface_formats[index_in_array].colorSpace,
    .imageExtent = surface_capabilities.currentExtent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
  }
}
