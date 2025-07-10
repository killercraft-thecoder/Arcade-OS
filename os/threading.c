#include <stdint.h>
#include <stdbool.h>
#include "./declaration/threading.h"

// Thread pool
static Thread threadPool[MAX_THREADS];
static uint8_t currentThread = 255; // Sentinel value for no active thread
static uint8_t threadCount = 0;     // Total number of threads

// Create a new thread with a specified priority
bool createThread(void (*task)(void), uint8_t priority)
{
    if (task == NULL)
    { // Ensure task is valid
        return false;
    }
    if (threadCount >= MAX_THREADS)
    { // Thread pool is full
        return false;
    }
    if (priority > MAX_PRIORITY)
    { // Enforce priority ceiling for non-system tasks
        return false;
    }
    threadPool[threadCount].id = threadCount;
    threadPool[threadCount].task = task;
    threadPool[threadCount].isBlocked = false;
    threadPool[threadCount].priority = priority; // Assign priority
    threadCount++;
    return true;
}

// Set priority for an existing thread
static bool setThreadPriority(uint8_t id, uint8_t priority)
{
    if (id >= threadCount || priority > MAX_PRIORITY)
    {
        return false; // Ensure thread ID and priority are valid
    }
    threadPool[id].priority = priority;
    return true;
}

// Switch to the next thread (priority-based round-robin)
bool switchThread()
{
    if (threadCount == 0)
    { // No threads to switch to
        return false;
    }

    uint8_t highestPriority = 0;
    int nextThreadIndex = -1;

    // Find the highest priority among unblocked threads
    for (uint8_t i = 0; i < threadCount; i++)
    {
        if (!threadPool[i].isBlocked && threadPool[i].priority > highestPriority)
        {
            highestPriority = threadPool[i].priority;
        }
    }

    // Find the next thread within the highest-priority group (round-robin fashion)
    for (uint8_t i = (currentThread + 1) % threadCount; i != currentThread; i = (i + 1) % threadCount)
    {
        if (!threadPool[i].isBlocked && threadPool[i].priority == highestPriority)
        {
            nextThreadIndex = i;
            break;
        }
    }

    // If no thread is found, return false
    if (nextThreadIndex == -1)
    {
        return false;
    }

    currentThread = nextThreadIndex; // Switch to the selected thread

    // Execute the thread's task
    threadPool[currentThread].task();

    return true;
}

// Block the current thread
static bool blockCurrentThread()
{
    if (currentThread != 255 && threadPool[currentThread].priority < MAX_PRIORITY)
    { // Check if a thread is active
        threadPool[currentThread].isBlocked = true;
        return true;
    }
    return false;
}

// Unblock a specific thread
static bool unblockThread(uint8_t id)
{
    if (id >= threadCount)
    { // Validate thread ID
        return false;
    }
    threadPool[id].isBlocked = false;
    return true;
}