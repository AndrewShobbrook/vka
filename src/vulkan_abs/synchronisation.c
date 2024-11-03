#include <assert.h>
#include <stdint.h>
#include <vulkan/vulkan_core.h>
#include <vulkan_abs/synchronisation.h>

vka_SyncStruct *vka_CreateSyncStruct();

vka_FenceHandle vka_CreateFence(vka_SyncStruct *sync, uint32_t signalled, VkDevice device) {
    VkFenceCreateInfo fence_create_info = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = signalled ? VK_FENCE_CREATE_SIGNALED_BIT : 0,
        .pNext = NULL,
    };
    VkFence underlying_fence = 0;
    VkResult result = vkCreateFence(device, &fence_create_info, NULL, &underlying_fence);
    assert(result == VK_SUCCESS)
}
