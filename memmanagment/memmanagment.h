#ifndef MEMMANAGMENT_H__
#define MEMMANAGMENT_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <semaphore.h>

#ifndef POOL_SIZE
#define POOL_SIZE 262144
#endif

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 16384
#endif

#if POOL_SIZE % BLOCK_SIZE != 0
#error "POOL_SIZE must be a multiple of BLOCK_SIZE"
#endif

#define NUM_BLOCKS (POOL_SIZE / BLOCK_SIZE)

typedef struct {
    uint8_t pool[POOL_SIZE];
    uint8_t block_status[NUM_BLOCKS];
    sem_t semaphore;
} MemoryAllocator;

void MemoryAllocator_init(MemoryAllocator *allocator);
void* MemoryAllocator_allocate(MemoryAllocator *allocator);
bool MemoryAllocator_free(MemoryAllocator *allocator, void *block);

#endif // MEMMANAGMENT_H__
