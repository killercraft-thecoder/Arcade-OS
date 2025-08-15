#ifndef ARCADEOS_KERNEL_H
#define ARCADEOS_KERNEL_H

#include <cstdint>
#include <vector>
#include <functional>

namespace Kernel {
    struct Task {
        uint32_t id;
        std::function<void()> function;
        bool visible;
    };

    void Init();
    void RegisterTask(std::function<void()> taskFunction, bool visible = true);
    void RunTasks();
    uint32_t* AllocateRAM(uint32_t size);
    bool DeallocateRAM(uint32_t* pointer);
    void TriggerGC();
    // Add more kernel services as needed
}

#endif
