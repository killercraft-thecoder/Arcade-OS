#include "VM.h"
#include "controlgc.cpp" // include GC
#include "os/declaration/OSconfig.h"
#include <iostream>

#ifndef TOTAL_PHYSICAL_MEMORY
#define TOTAL_PHYSICAL_MEMORY (OS_RAM_SIZE_KB * 1024) // Use value from OSConfig.h

#endif

// Page Class Implementation
Page::Page(uint32_t vAddress, uint32_t pAddress)
    : virtualAddress(vAddress), physicalAddress(pAddress), allocated(true) {}

uint32_t Page::getVirtualAddress() const { return virtualAddress; }
uint32_t Page::getPhysicalAddress() const { return physicalAddress; }
bool Page::isAllocated() const { return allocated; }
void Page::deallocate() { allocated = false; }

// PageManager Class Implementation
PageManager::PageManager() : currentPhysicalAddress(0x00001000) {}

bool PageManager::allocatePage(uint32_t vAddress)
{
    if (currentPhysicalAddress + 0x2000 > TOTAL_PHYSICAL_MEMORY)
    {
        std::cerr << "Error: Out of physical memory!\n";
        return false;
    }
    uint32_t pAddress = currentPhysicalAddress;
    currentPhysicalAddress += 0x2000; // Increment by 8KB
    Page newPage(vAddress, pAddress);
    pages.push_back(newPage);
    std::cout << "Allocated page: Virtual " << vAddress << ", Physical " << pAddress << "\n";
    return true;
}

Page *PageManager::findPage(uint32_t vAddress)
{
    for (auto &page : pages)
    {
        if (page.getVirtualAddress() == vAddress)
        {
            return &page;
        }
    }
    return nullptr;
}

bool PageManager::deallocatePage(uint32_t vAddress)
{
    Page *page = findPage(vAddress);
    if (page)
    {
        page->deallocate();
        control::gc();
        std::cout << "Deallocated page: Virtual " << vAddress << "\n";
        return true;
    }
    else
    {
        std::cout << "Page not found for Virtual " << vAddress << "\n";
        return false;
    }
}

uint32_t PageManager::translateAddress(uint32_t vAddress)
{
    Page *page = findPage(vAddress);
    if (page && page->isAllocated())
    {
        return page->getPhysicalAddress();
    }
    std::cout << "Invalid Virtual Address: " << vAddress << "\n";
    return 0xFFFFFFFF;
}

// If you add any RAM allocation in VM.cpp, use Kernel::AllocateRAM(size) and Kernel::DeallocateRAM(pointer)
