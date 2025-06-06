#ifndef VM_H
#define VM_H

#include <cstdint>
#include <vector>

// Page Class
class Page
{
private:
    uint32_t virtualAddress;
    uint32_t physicalAddress;
    bool allocated;

public:
    Page(uint32_t vAddress, uint32_t pAddress);
    uint32_t getVirtualAddress() const;
    uint32_t getPhysicalAddress() const;
    bool isAllocated() const;
    void deallocate();
};

// PageManager Class
class PageManager
{
private:
    std::vector<Page> pages;
    uint32_t currentPhysicalAddress;

public:
    PageManager();
    void allocatePage(uint32_t vAddress);
    Page *findPage(uint32_t vAddress);
    void deallocatePage(uint32_t vAddress);
    uint32_t translateAddress(uint32_t vAddress);
};

#endif // VM_H