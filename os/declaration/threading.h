#ifndef ARCADEOS_THREADING_H
#define ARCADEOS_THREADING_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_THREADS 8
#define MAX_PRIORITY 250    // Non-system priority limit
#define SYSTEM_PRIORITY 254 // Reserved priority for system-critical tasks

/**
 * @typedef Thread
 * @brief Represents a lightweight thread/task in ARCADEOS.
 *
 * Each thread is a function pointer with optional metadata like blocking status and priority.
 */
typedef struct
{
    uint8_t id;         /**< Unique thread ID (assigned at creation) */
    void (*task)(void); /**< Task function to be executed by the thread */
    bool isBlocked;     /**< Indicates if the thread is currently blocked */
    uint8_t priority;   /**< Thread priority (0–255) */
} Thread;

/**
 * @function createThread
 * @brief Creates and registers a new thread.
 *
 * @param task Function pointer to the thread task.
 * @param priority Priority level (must be ≤ MAX_PRIORITY).
 * @return `true` on success, `false` if thread pool is full or parameters are invalid.
 */
bool createThread(void (*task)(void), uint8_t priority);

/**
 * @function switchThread
 * @brief Switches execution to the next unblocked thread with the highest priority.
 *
 * This uses a priority-based round-robin mechanism.
 * @return `true` if a thread was successfully switched, `false` otherwise.
 */
bool switchThread();

/**
 * @function unblockThread
 * @brief Unblocks a thread by ID.
 *
 * @param id Thread ID to unblock.
 * @return `true` if the thread exists and was unblocked, `false` otherwise.
 */
bool unblockThread(uint8_t id);

/**
 * @function blockCurrentThread
 * @brief Blocks the currently active thread.
 *
 * Only works for non-system threads.
 * @return `true` if blocked successfully, `false` otherwise.
 */
static bool blockCurrentThread();

/**
 * @function setThreadPriority
 * @brief Sets a new priority level for a thread.
 *
 * @param id Thread ID.
 * @param priority New priority value (≤ MAX_PRIORITY).
 * @return `true` if updated successfully, `false` otherwise.
 */
static bool setThreadPriority(uint8_t id, uint8_t priority);

#endif // ARCADEOS_THREADING_H