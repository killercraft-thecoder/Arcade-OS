#ifndef OS_ALLOC_H
#define OS_ALLOC_H

#include <stddef.h>
#include "CodalHeapAllocator.h" // Codal's heap interface

namespace OS
{
    // Allocate memory
    static inline void *Malloc(size_t size)
    {
        return device_malloc(size);
    }

    // Free memory
    static inline void Free(void *ptr)
    {
        device_free(ptr);
    }

    // Resize memory
    static inline void *Realloc(void *ptr, size_t size)
    {
        return device_realloc(ptr, size);
    }

    static inline uint32_t Heap_size(uint8_t index)
    {
        return device_heap_size(index);
    }
}

#endif // OS_ALLOC_H