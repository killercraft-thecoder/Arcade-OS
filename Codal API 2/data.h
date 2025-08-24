#ifndef MEM_UTILS_H
#define MEM_UTILS_H

#define MARK(v)                           \
    do {                                  \
        GC_CHECK(inGCArea(v), 42);        \
        *(uintptr_t *)(v) |= MARKED_MASK; \
    } while (0)

#define UNMARK(v)                          \
    do {                                   \
        GC_CHECK(inGCArea(v), 43);         \
        *(uintptr_t *)(v) &= ~MARKED_MASK; \
    } while (0)

#include <stdint.h>
#include <stddef.h>
#include <string.h> // fallback for memcpy/memset
#include "gc.h"

// Compiler detection for built-ins
#if defined(__GNUC__) || defined(__clang__)
    #define MEMCPY_BUILTIN(dest, src, len) __builtin_memcpy((dest), (src), (len))
    #define MEMSET_BUILTIN(ptr, val, len)  __builtin_memset((ptr), (val), (len))
#else
    #define MEMCPY_BUILTIN(dest, src, len) memcpy((dest), (src), (len))
    #define MEMSET_BUILTIN(ptr, val, len)  memset((ptr), (val), (len))
#endif

// ===== Core versions =====

/**
 * @brief Copy a block of memory from src to dest, then zero out the source.
 *
 * @param src  Pointer to the source data.
 * @param dest Pointer to the destination buffer.
 * @param len  Number of bytes to copy.
 *
 * @note This function is not GC-aware. Use mem_move_and_zero_gc() for GC-managed memory.
 */
static inline void mem_move_and_zero(void *src, void *dest, size_t len)
{
    if (!src || !dest || len == 0) return;
    MEMCPY_BUILTIN(dest, src, len);
    MEMSET_BUILTIN(src, 0, len);
}

/**
 * @brief Copy a block of memory from src to dest without modifying the source.
 *
 * @param src  Pointer to the source data.
 * @param dest Pointer to the destination buffer.
 * @param len  Number of bytes to copy.
 *
 * @note This function is not GC-aware. Use mem_clone_gc() for GC-managed memory.
 */
static inline void mem_clone(const void *src, void *dest, size_t len)
{
    if (!src || !dest || len == 0) return;
    MEMCPY_BUILTIN(dest, src, len);
}

/**
 * @brief Zero out a block of memory.
 *
 * @param ptr Pointer to the memory block.
 * @param len Number of bytes to clear.
 *
 * @note This function is not GC-aware. Use mem_zero_gc() for GC-managed memory.
 */
static inline void mem_zero(void *ptr, size_t len)
{
    if (!ptr || len == 0) return;
    MEMSET_BUILTIN(ptr, 0, len);
}

/**
 * @brief Fill a block of memory with a specific byte value.
 *
 * @param ptr   Pointer to the memory block.
 * @param value Byte value to set.
 * @param len   Number of bytes to set.
 *
 * @note This function is not GC-aware.
 */
static inline void mem_fill(void *ptr, int value, size_t len)
{
    if (!ptr || len == 0) return;
    MEMSET_BUILTIN(ptr, value, len);
}

// ===== GC-aware versions =====

/**
 * @brief GC-aware: Move memory from src to dest, zero out src, update GC marks.
 *
 * @param src  Pointer to the GC-managed source object.
 * @param dest Pointer to the GC-managed destination object.
 * @param len  Number of bytes to copy.
 *
 * @warning Only use with GC-managed heap objects. Passing stack or static memory
 *          may cause GC_CHECK to fail.
 */
static inline void mem_move_and_zero_gc(void *src, void *dest, size_t len)
{
    if (!src || !dest || len == 0) return;

    // Copy data
    MEMCPY_BUILTIN(dest, src, len);

    // Clear original
    MEMSET_BUILTIN(src, 0, len);

    // GC bookkeeping: mark new location, unmark old
    MARK(dest);
    UNMARK(src);
}

/**
 * @brief GC-aware: Clone memory from src to dest, mark both as live.
 *
 * @param src  Pointer to the GC-managed source object.
 * @param dest Pointer to the GC-managed destination object.
 * @param len  Number of bytes to copy.
 *
 * @warning Only use with GC-managed heap objects.
 */
static inline void mem_clone_gc(const void *src, void *dest, size_t len)
{
    if (!src || !dest || len == 0) return;

    MEMCPY_BUILTIN(dest, src, len);

    // GC bookkeeping: mark both
    MARK(dest);
    MARK(src);
}

/**
 * @brief GC-aware: Zero out a GC-managed memory block and unmark it.
 *
 * @param ptr Pointer to the GC-managed object.
 * @param len Number of bytes to clear.
 *
 * @warning Only use with GC-managed heap objects.
 */
static inline void mem_zero_gc(void *ptr, size_t len)
{
    if (!ptr || len == 0) return;

    MEMSET_BUILTIN(ptr, 0, len);

    // GC bookkeeping: unmark zeroed block
    UNMARK(ptr);
}


#endif // MEM_UTILS_H