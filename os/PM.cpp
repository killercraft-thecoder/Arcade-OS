#include "PM.h"
#include "frame.h" // part of the screen API
#include "Timer.h"

namespace PM
{

    inline void EnterSleepState(uint16_t durationInMinutes)
    {
        StopDisplayUpdates(); // Freeze the display to save power

        target_wait((durationInMinutes * 60) * 1000) // sleep.

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