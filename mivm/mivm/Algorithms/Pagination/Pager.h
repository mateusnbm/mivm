//
//  Pager.h
//  Mom
//
//  Created by Veve on 31/08/14.
//  Copyright (c) 2014 Lits Development. All rights reserved.
//

#ifndef MOM_PAGER
#define MOM_PAGER

#include <stdio.h>

#include "Defs.h"

#include "PageManager.h"
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

class Pager:public PageManager {
    private:
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
        Pager(Memory *memory, Disk *disk, size_t pageSize, size_t *error):PageManager(memory, disk, pageSize, error){}
        void init(size_t memorySize, size_t diskSize);
        void newPages(size_t pid, size_t address, size_t pagesCount);
        size_t translateAddress(size_t virtualAddress);
};

#endif
