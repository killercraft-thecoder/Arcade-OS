// ARCADEOS Bootloader/Main Entry
#include "../os/declaration/OS.h"
#include "../os/declaration/OSconfig.h"
#include "PIL.h"
#include "platform.h"
#include "pxt.h"
#include "ui.h"

// Platform/hardware initialization abstraction
void board_init()
{
    // Initialize platform (timers, pins, etc.)
    pxt::platform_init();
    // Add board-specific initialization here if needed
}

int main()
{
    std::cout << "Booting " << get_os_name() << " v" << OS_VERSION << " on " << TARGET_BOARD << std::endl; // TEhcincaly iostream does not exist on hardware.

    // Initialize CODAL device/fiber/event systems (required for high-level OS features)
    // If you add custom device init, do it here before mounting the filesystem
    board_init();
    std::cout << "Platform initialized." << std::endl;

    // Mount the filesystem
    std::cout << "Mounting filesystem..." << std::endl;
    PIL::Mount();
    std::cout << "Filesystem mounted." << std::endl;

    // Register a simple task (demo)
    PIL::RegisterTask([]()
                      { std::cout << "Hello from a registered task!" << std::endl; }, true);

    // Main OS loop (simple demo)
    while (true)
    {
        PIL::RunTasks();
        // TODO: Add sleep/yield, interrupt, or event handling here
        break; // Remove this break for a real OS loop
    }

    std::cout << "ARCADEOS shutdown." << std::endl;
    PIL::CreateConfigFile("/usr/games/thing/thing.config", 0); // would be skipped if already made. , the second arg is the appID , whichshould proalby be removed maybe in favor of MPU?
    PIL::File configFile = PIL::open("/usr/games/thing/thing.config", 0, "r");
    pxt::Buffer = configFile.read();
    /// now we need to decode the raw bytes. , or myabe i implmented the os wron g but whatever.
    return 0;
}