#include <iostream>
#include <thread>
#include <chrono>
#include <functional>

namespace screen
{
    std::function<void()> update_callback = nullptr;
    bool updated = false;
    bool sleepmode = false;   // Sleep mode flag
    uint8_t frameCounter = 0; // Tracks frames in sleep mode (optimized with uint8)

    void update()
    {
        if (sleepmode)
        {
            // Only update the screen every 60 frames in sleep mode
            frameCounter++;
            if (frameCounter >= 60)
            {
                if (update_callback)
                {
                    update_callback();
                }
                updated = true;
                frameCounter = 0; // Reset the frame counter
            }
            return; // Exit early in sleep mode
        }

        // Normal update behavior in active mode
        if (update_callback)
        {
            update_callback();
        }
        updated = true;
    }

    void setup_update(std::function<void()> update)
    {
        updated = true;
        update_callback = update;
        update(); // Immediate refresh on setup
    }

    void fallback_refresh()
    {
        while (true)
        {
            updated = false;
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Approx. 60 FPS timing
            screen::update();                                           // Trigger updates based on the current mode
        }
    }

    void start_fallback_refresh()
    {
        std::thread(fallback_refresh).detach();
    }

    void wake()
    {
        sleepmode = false;
        frameCounter = 0; // Reset frame counter when exiting sleep mode
        std::cout << "Screen: Exiting sleep mode and resuming normal updates.\n";
    }

    void enter_sleep()
    {
        sleepmode = true;
        frameCounter = 0; // Initialize frame counter for sleep mode
        std::cout << "Screen: Entering sleep mode (updates every 60 frames).\n";
    }

    void timed_sleep(uint32_t durationInMilliseconds)
    {
        enter_sleep();
        std::this_thread::sleep_for(std::chrono::milliseconds(durationInMilliseconds)); // Simulate sleep duration
        wake();                                                                         // Automatically exit sleep mode after duration
    }
}