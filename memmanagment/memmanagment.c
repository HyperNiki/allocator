#include "memmanagment.h"

// Initialize the memory allocator
void MemoryAllocator_init(MemoryAllocator *allocator) {
    for (size_t i = 0; i < NUM_BLOCKS; i++) {
        allocator->block_status[i] = 0; // 0 indicates that the block is free
    }
    sem_init(&allocator->semaphore, 0, 1); // Initialize semaphore
}

// Allocate a block from the memory pool
void* MemoryAllocator_allocate(MemoryAllocator *allocator) {
    for (size_t i = 0; i < NUM_BLOCKS; i++) {
        sem_wait(&allocator->semaphore); // Lock
        if (allocator->block_status[i] == 0) { // If the block is free
            allocator->block_status[i] = 1; // Mark the block as allocated
            sem_post(&allocator->semaphore); // Unlock
            return &allocator->pool[i * BLOCK_SIZE];
        }
        sem_post(&allocator->semaphore); // Unlock
    }
    return NULL; // No free blocks available
}

// Free a previously allocated block
bool MemoryAllocator_free(MemoryAllocator *allocator, void *block) {
    uintptr_t block_address = (uintptr_t)block;
    uintptr_t pool_start = (uintptr_t)allocator->pool;
    uintptr_t pool_end = pool_start + POOL_SIZE;

    // Ensure the block address is within the bounds of the pool
    if (block_address >= pool_start && block_address < pool_end) {
        size_t index = (block_address - pool_start) / BLOCK_SIZE;
        
        sem_wait(&allocator->semaphore); // Lock
        allocator->block_status[index] = 0; // Mark the block as free
        sem_post(&allocator->semaphore); // Unlock
        
        return true;
    }
    return false;
}
