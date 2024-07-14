#include "memmanagment.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

// Declare a static instance of the memory allocator
static MemoryAllocator allocator;

// Function to be used by threads for allocation and freeing memory
void* allocate_and_write(void* arg) {
    for (int t = 0; t < NUM_BLOCKS; t++)
    {
        
        void *block = MemoryAllocator_allocate(&allocator);
        while (block == NULL)   // wait block
        {
            block = MemoryAllocator_allocate(&allocator);
        } 
        
        assert(block != NULL);

        // Test writing to each byte of the allocated block
        uint8_t *data = (uint8_t*)block;
        for (size_t i = 0; i < BLOCK_SIZE; i++) {
            data[i] = (uint8_t)i;
            assert(data[i] == (uint8_t)i); // Ensure the data was written correctly
        }

        assert(MemoryAllocator_free(&allocator, block) == true);
    }
    return NULL;
}

// Unit test for allocation and freeing of blocks
void test_allocation_and_freeing() {
    MemoryAllocator_init(&allocator);

    clock_t start = clock();

    void *block1 = MemoryAllocator_allocate(&allocator);
    void *block2 = MemoryAllocator_allocate(&allocator);

    // Ensure blocks are allocated
    assert(block1 != NULL);
    assert(block2 != NULL);

    // Ensure blocks are different
    assert(block1 != block2);

    // Free the first block and allocate again
    assert(MemoryAllocator_free(&allocator, block1) == true);
    void *block3 = MemoryAllocator_allocate(&allocator);

    // Ensure the new allocation reuses the freed block
    assert(block1 == block3);

    clock_t stop = clock();
    double total_time = (double)(stop - start) / CLOCKS_PER_SEC;
    printf("Test allocation and freeing passed! Time: %.05fs\n", total_time);
}

// Unit test for exhaustion of the memory pool
void test_exhaustion_of_pool() {
    MemoryAllocator_init(&allocator);

    clock_t start = clock();

    void *blocks[NUM_BLOCKS];

    // Allocate all blocks
    for (size_t i = 0; i < NUM_BLOCKS; i++) {
        blocks[i] = MemoryAllocator_allocate(&allocator);
        assert(blocks[i] != NULL);
    }

    // Ensure no more blocks can be allocated
    void *extra_block = MemoryAllocator_allocate(&allocator);
    assert(extra_block == NULL);

    // Free all blocks
    for (size_t i = 0; i < NUM_BLOCKS; i++) {
        assert(MemoryAllocator_free(&allocator, blocks[i]) == true);
    }

    clock_t stop = clock();
    double total_time = (double)(stop - start) / CLOCKS_PER_SEC;
    printf("Test exhaustion of pool passed! Time: %.05fs\n", total_time);
}

// Unit test for writing to each byte of allocated memory
void test_memory_writing() {
    MemoryAllocator_init(&allocator);

    clock_t start = clock();

    void *block = MemoryAllocator_allocate(&allocator);
    assert(block != NULL);

    // Test writing to each byte of the allocated block
    uint8_t *data = (uint8_t*)block;
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        data[i] = (uint8_t)i;
        assert(data[i] == (uint8_t)i); // Ensure the data was written correctly
    }

    assert(MemoryAllocator_free(&allocator, block) == true);

    clock_t stop = clock();
    double total_time = (double)(stop - start) / CLOCKS_PER_SEC;
    printf("Test memory writing passed! Time: %.05fs\n", total_time);
}

// Function to run the multithreaded test
void test_multithreaded() {
    MemoryAllocator_init(&allocator);

    clock_t start = clock();

    const int num_threads = 10;
    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, allocate_and_write, NULL);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t stop = clock();
    double total_time = (double)(stop - start) / CLOCKS_PER_SEC;
    printf("Multithreaded test passed! Time: %.05fs\n", total_time);
}

int main(void) {
    printf("Running unit tests...\n");
    test_allocation_and_freeing();
    test_exhaustion_of_pool();
    test_memory_writing();
    printf("All unit tests passed!\n");

    printf("Running multithreaded unit test...\n");
    test_multithreaded();
    printf("All tests passed!\n");

    return 0;
}
