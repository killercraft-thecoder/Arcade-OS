#include "PM.h"
#include "frame.h" // part of the screen API
#include <thread>
#include <chrono>

namespace PM
{

    void EnterSleepState(uint16_t durationInMinutes)
    {
        StopDisplayUpdates(); // Freeze the display to save power

        std::this_thread::sleep_for(std::chrono::minutes(durationInMinutes)); // Simulate the sleep duration

        WakeUp(); // Automatically wake up after the sleep duration
    }

    static inline void WakeUp()
    {
        screen::sleepmode = false
    }

    static inline void StopDisplayUpdates()
    {
        screen::sleepmode = true
    }
}

// If you add any RAM allocation in PM.cpp, use Kernel::AllocateRAM(size) and Kernel::DeallocateRAM(pointer)