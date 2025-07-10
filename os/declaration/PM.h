#ifndef PM_H
#define PM_H

#include <cstdint>

namespace PM {
    void EnterSleepState(uint32_t durationInMinutes); // Enter sleep mode for a set duration
}

#endif // PM_H