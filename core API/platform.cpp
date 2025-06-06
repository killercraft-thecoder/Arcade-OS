#include "pxt.h"
// STM32-only platform initialization
namespace pxt {

// Add STM32-specific timer, peripherals, etc.
CODAL_TIMER devTimer;

void platform_init() {
    // STM32-specific hardware initialization
    // (Timers, pins, clocks, etc.)
    // Add board-specific code here if needed
}

} // namespace pxt
