#include "Kernel.h"
#include "core API/gc.h"
#include "os/declaration/OSconfig.h"
#include <algorithm>

namespace Kernel
{
    static std::vector<Task> taskList;
    static uint32_t currentTaskID = 0;
    constexpr size_t OS_RAM_SIZE = OS_RAM_SIZE_KB * 1024;

    void Init()
    {
        // Initialize kernel state, hardware, etc.
        taskList.clear();
        currentTaskID = 0;
    }

    void RegisterTask(std::function<void()> taskFunction, bool visible)
    {
        Task newTask = {currentTaskID++, taskFunction, visible};
        taskList.push_back(newTask);
    }

    void RunTasks()
    {
        int tasksRunThisFrame = 0;
        const int tasksPerCycle = 3;
        for (auto &task : taskList)
        {
            if (!task.visible)
                continue;
            task.function();
            tasksRunThisFrame++;
            if (tasksRunThisFrame >= tasksPerCycle)
            {
                tasksRunThisFrame = 0;
            }
        }
    }

    uint32_t *AllocateRAM(uint32_t size)
    {
        // Prevent allocation if it would exceed OS_RAM_SIZE
        static size_t used_ram = 0;
        if (used_ram + size > OS_RAM_SIZE)
        {
            std::cerr << "[Kernel] RAM allocation failed: not enough memory (requested " << size << ", available " << (OS_RAM_SIZE - used_ram) << ")\n";
            return nullptr;
        }
        uint32_t *pointer = new (std::nothrow) uint32_t[size / sizeof(uint32_t)];
        if (pointer)
        {
            used_ram += size;
        }
        return pointer;
    }

    bool DeallocateRAM(uint32_t *pointer)
    {
        if (pointer)
        {
            // For simplicity, assume all allocations are the same size (not tracked per pointer)
            // In a real OS, you'd track allocation sizes per pointer
            delete[] pointer;
            // used_ram -= size; // Not decremented here due to lack of size tracking
            return true;
        }
        return false;
    }

    inline int TriggerGC()
    {
        // Call the core API GC
        int freed = core::gc();
        return freed;
    }
}
