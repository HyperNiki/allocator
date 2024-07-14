# Memory Block Allocator Library

This C library provides a memory block allocator module that allows for allocating memory in fixed-size blocks from a static memory pool. The block and pool sizes are configurable at build time and can be adapted for different embedded platforms with various bit widths in a multitasking environment (RTOS). 

## Features
- Allocation of memory in fixed-size blocks
- Configurable block and pool sizes at build time
- Adapted for embedded platforms with different bit widths
- Suitable for use in a multitasking environment (RTOS)

## Building
To build the library, use the provided build script - build.sh. You can configure the block and pool sizes by editing the configuration file memmanagment.h.

## Testing
The library includes a set of unit tests to ensure its functionality. The unit tests cover basic allocation and deallocation scenarios.