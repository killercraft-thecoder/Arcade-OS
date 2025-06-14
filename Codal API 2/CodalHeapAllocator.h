/*
The MIT License (MIT)

Copyright (c) 2017 Lancaster University.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

/**
 * A simple 32 bit block based memory allocator. This allows one or more memory segments to
 * be designated as heap storage, and is designed to run in a static memory area or inside the standard C
 * heap for use by the codal device runtime. This is required for several reasons:
 *
 * 1) It reduces memory fragmentation due to the high churn sometime placed on the heap
 * by ManagedTypes, fibers and user code. Underlying heap implentations are often have very simplistic
 * allocation pilicies and suffer from fragmentation in prolonged use - which can cause programs to
 * stop working after a period of time. The algorithm implemented here is simple, but highly tolerant to
 * large amounts of churn.
 *
 * 2) It allows us to reuse the 8K of SRAM set aside for SoftDevice as additional heap storage
 * when BLE is not in use.
 *
 * 3) It gives a simple example of how memory allocation works! :-)
 *
 * P.S. This is a very simple allocator, therefore not without its weaknesses. Why don't you consider
 * what these are, and consider the tradeoffs against simplicity...
 *
 * @note The need for this should be reviewed in the future, if a different memory allocator is
 * made available in the mbed platform.
 *
 * TODO: Consider caching recently freed blocks to improve allocation time.
 */

#ifndef DEVICE_HEAP_ALLOCTOR_H
#define DEVICE_HEAP_ALLOCTOR_H

#include "CodalConfig.h"

// Flag to indicate that a given block is FREE/USED (top bit of a CPU word)
#define DEVICE_HEAP_BLOCK_FREE (1 << (sizeof(PROCESSOR_WORD_TYPE) * 8 - 1))
#define DEVICE_HEAP_BLOCK_SIZE (sizeof(PROCESSOR_WORD_TYPE))

struct HeapDefinition
{
  PROCESSOR_WORD_TYPE *heap_start; // Physical address of the start of this heap.
  PROCESSOR_WORD_TYPE *heap_end;   // Physical address of the end of this heap.
};
extern PROCESSOR_WORD_TYPE codal_heap_start;

/**
 * Create and initialise a given memory region as for heap storage.
 * After this is called, any future calls to malloc, new, free or delete may use the new heap.
 * The heap allocator will attempt to allocate memory from heaps in the order that they are created.
 * i.e. memory will be allocated from first heap created until it is full, then the second heap, and so on.
 *
 * @param start The start address of memory to use as a heap region.
 *
 * @param end The end address of memory to use as a heap region.
 *
 * @return DEVICE_OK on success, or DEVICE_NO_RESOURCES if the heap could not be allocated.
 *
 * @note Only code that #includes DeviceHeapAllocator.h will use this heap. This includes all codal device runtime
 * code, and user code targetting the runtime. External code can choose to include this file, or
 * simply use the standard heap.
 */
int device_create_heap(PROCESSOR_WORD_TYPE start, PROCESSOR_WORD_TYPE end);

/**
 * Returns the size of a given heap.
 *
 * @param heap_index index between 0 and DEVICE_MAXIMUM_HEAPS-1
 *
 * @return the size of heap in bytes, or zero if no such heap exists.
 */
uint32_t device_heap_size(uint8_t heap_index);

/**
 * Attempt to allocate a given amount of memory from any of our configured heap areas.
 *
 * @param size The amount of memory, in bytes, to allocate.
 *
 * @return A pointer to the allocated memory, or NULL if insufficient memory is available.
 */
extern "C" void *device_malloc(size_t size);

/**
 * Release a given area of memory from the heap.
 *
 * @param mem The memory area to release.
 */
extern "C" void device_free(void *mem);

/**
 * Copy existing contents of ptr to a new memory block of given size.
 *
 * @param ptr The existing memory block (can be NULL)
 * @param size The size of new block (can be smaller or larger than the old one)
 */
extern "C" void *device_realloc(void *ptr, size_t size);

#endif
