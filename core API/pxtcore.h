#ifndef __PXTCORE_H
#define __PXTCORE_H

#include "CodalDmesg.h"
#include "CodalHeapAllocator.h"

#define PXT_CODAL 1 // Indicates Codal integration for the platform

#define itoa(a, b) codal::itoa(a, b) // Codal's integer-to-string implementation

// Garbage Collection and Memory Management
#define GC_GET_HEAP_SIZE() device_heap_size(0) // Retrieve heap size
#define GC_STACK_BASE DEVICE_STACK_BASE        // Define the stack base
#define xmalloc device_malloc                  // Use Codal's malloc
#define xfree device_free                      // Use Codal's free

// Ensure maximum allocation size to prevent memory fragmentation
#define GC_MAX_ALLOC_SIZE (64 * 1024) // Reduced for tighter STM32F4 memory constraints

// Additional configurations (if needed for F4 performance tuning)
// Uncomment or add specific adjustments if hardware demands it:
// #define DEVICE_USE_CACHE 1 // Example: enable cache on supported F4 hardware
// #define DEVICE_USE_HARDFAULT_HANDLING 1 // Example: handle memory issues safely

#endif
