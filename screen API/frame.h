#ifndef FRAME_H
#define FRAME_H

#include <iostream>
#include <thread>
#include <chrono>
#include <functional>

namespace screen
{
    extern std::function<void()> update_callback;
    extern bool updated;
    extern bool sleepmode;
    extern uint8_t frameCounter;

    void update();
    void setup_update(std::function<void()> update);
    void fallback_refresh();
    void start_fallback_refresh();
    void wake();
    void enter_sleep();
    void timed_sleep(uint32_t durationInMilliseconds);
}

#endif // FRAME_H