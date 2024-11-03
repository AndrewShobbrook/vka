#pragma once
#include <stdint.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

typedef uint64_t vka_FenceHandle;
typedef uint64_t vka_SemaphoreHandle;
typedef uint64_t SubmissionHandle;

typedef struct {
    uint32_t fence_count;
    VkFence *fences;
    uint32_t semaphore_count;
    VkSemaphore *semaphores;
} vka_SyncStruct;
