//
//  MemManager.hpp
//

#ifndef MemManager_hpp
#define MemManager_hpp

#include <stdio.h>

#include "Defs.h"

#include "Memory.h"
#include "Disk.h"

typedef struct item {
    size_t virtualIndex;
    size_t physicalIndex;
    struct item *next;
} Item;

typedef struct page {
    size_t pid;
    bool valid;
    bool dirty;
    bool onMemory;
    size_t physicalIndex;
}Page;

class MemManager {
private:
    Memory * _memory;
    Disk * _disk;
    
    size_t slotsCount;
    size_t emptySlots;
    Page *virtualMap;
    size_t *physicalMap;
    size_t availableIndex();
    Item *list;
    Item *newItem(size_t virtualIndex, size_t physicalIndex);
    Item *pushItem(Item *list, Item *item);
    Item *popItem(Item **list);
    
public:
    size_t _pageSize;
    MemManager(Memory *memory, Disk *disk, size_t pageSize, size_t *error);
    void writePageOnDisk(size_t virtualAddress, size_t physicalAddress, size_t *error);
    void writePageOnMemory(size_t virtualAddress, size_t physicalAddress, size_t *error);
    void *retrieveData(size_t virtualAddress, size_t size, size_t *error);
    void storeData(size_t virtualAddress, size_t data, size_t size, size_t *error);
    
    void newPages(size_t pid, size_t address, size_t pagesCount);
    size_t translateAddress(size_t virtualAddress);
    
};

#endif /* MemManager_hpp */
