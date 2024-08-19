#include "vulkan_abs/swapchain.h"
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

vka_SwapchainReturn vka_CreateSwapchain(VkPhysicalDevice phys_device, VkDevice device,
                                        GLFWwindow *window, VkSurfaceKHR surface,
                                        VkSwapchainKHR old_swapchain) {
  vka_SwapchainReturn swap_return = {0};
  uint32_t count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(phys_device, surface, &count, NULL);
  VkSurfaceFormatKHR *surface_formats = malloc(count * sizeof(VkSurfaceFormatKHR));
  vkGetPhysicalDeviceSurfaceFormatsKHR(phys_device, surface, &count, surface_formats);

  uint32_t index_found = -1;
  uint32_t index_in_array = -1;
  VkFormat desired_formats[] = {VK_FORMAT_R8G8B8A8_SRGB};


  // Selecting surface format
  for (int i = 0; i < count; i++) {
#ifndef NDEBUG
    printf("Format %d: %d, %d\n", i, surface_formats[i].format, surface_formats[i].colorSpace);
#endif
    for (int j = 0; j < 1; j++) {
      if (surface_formats[i].format == desired_formats[j] && surface_formats[i].colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
        if (j < index_found) {
          index_found = j;
          index_in_array = i;
        }
      }
    }
  }

  #ifndef NDEBUG
  printf("i: %d, j: %d\n", index_in_array, index_found);
  #endif

  count = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(phys_device, surface, &count, NULL);
  VkPresentModeKHR *present_modes = malloc(count * sizeof(count));

  VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;

  for (int i = 0; i < count; i++) {
    if (present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
  }
  
  VkSurfaceCapabilitiesKHR surface_capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phys_device, surface, &surface_capabilities);

  VkExtent2D extent = {0};

  if (surface_capabilities.currentExtent.width == 0xffffffff) {
    glfwGetFramebufferSize(window, &extent.width, &extent.height);
  } else {
    extent = surface_capabilities.currentExtent;
  }

  VkSwapchainCreateInfoKHR create_info = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext = NULL,
    .flags = 0,
    .surface = surface,
    .minImageCount = surface_capabilities.minImageCount,
    .imageFormat = surface_formats[index_in_array].format,
    .imageColorSpace = surface_formats[index_in_array].colorSpace,
    .imageExtent = extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = NULL,
    .preTransform = surface_capabilities.currentTransform,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .presentMode = present_mode,
    .clipped = VK_FALSE,
    .oldSwapchain = old_swapchain,
  };

  vkCreateSwapchainKHR(device, &create_info, NULL, &swap_return.swapchain);

  vkGetSwapchainImagesKHR(device, swap_return.swapchain, &swap_return.count, NULL);

  swap_return.images = malloc(swap_return.count * sizeof(VkImage));

  vkGetSwapchainImagesKHR(device, swap_return.swapchain,
                          &swap_return.count, swap_return.images);

  swap_return.extent = extent;
  swap_return.format = surface_formats[index_in_array];


  return swap_return;
}

VkImageView *vka_CreateSwapchainImageViews(VkDevice device, vka_SwapchainReturn swap_return) {
  VkImageView *image_views = malloc(swap_return.count * sizeof(VkImageView));

  for (int i = 0; i < swap_return.count; i++) {
    VkImageViewCreateInfo create_info = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .image = swap_return.images[i],
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = swap_return.format.format,
      .components = {
        .r = VK_COMPONENT_SWIZZLE_R,
        .g = VK_COMPONENT_SWIZZLE_G,
        .b = VK_COMPONENT_SWIZZLE_B,
        .a = VK_COMPONENT_SWIZZLE_A,
      },
      .subresourceRange = {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .layerCount = 1,
        .levelCount = 1,
        .baseMipLevel = 0,
        .baseArrayLayer = 0,
      }
    };

    vkCreateImageView(device, &create_info, NULL, &image_views[i]);
  }

  return image_views;
}
