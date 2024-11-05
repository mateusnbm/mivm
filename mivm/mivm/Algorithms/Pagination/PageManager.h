//
//  PageManager.h
//  Mom
//
//  Created by Veve on 01/09/14.
//  Copyright (c) 2014 Lits Development. All rights reserved.
//

#ifndef MOM_PAGEMANAGER
#define MOM_PAGEMANAGER

#include <stdio.h>

#include "Defs.h"
#include "Memory.h"
#include "Disk.h"

class PageManager {
    private:
        Memory *_memory;
        Disk *_disk;
    public:
        size_t _pageSize;
        PageManager(Memory *memory, Disk *disk, size_t pageSize, size_t *error);
        void writePageOnDisk(size_t virtualAddress, size_t physicalAddress, size_t *error);
        void writePageOnMemory(size_t virtualAddress, size_t physicalAddress, size_t *error);
        void *retrieveData(size_t virtualAddress, size_t size, size_t *error);
        void storeData(size_t virtualAddress, size_t data, size_t size, size_t *error);
    
        virtual void newPages(size_t pid, size_t address, size_t pagesCount) = 0;
        virtual size_t translateAddress(size_t virtualAddress) = 0;
};

#endif
