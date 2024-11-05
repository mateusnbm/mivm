//
//  PageManager.cpp
//  Mom
//
//  Created by Veve on 01/09/14.
//  Copyright (c) 2014 Lits Development. All rights reserved.
//

#include "PageManager.h"

PageManager::PageManager(Memory *memory, Disk *disk, size_t pageSize, size_t *error) {
    if (disk == NULL || memory == NULL) {
        *error = kErrorNullPointer;
    }else{
        _memory = memory;
        _disk = disk;
        _pageSize = pageSize;
        *error = 0;
    }
}

void PageManager::writePageOnDisk(size_t virtualAddress, size_t physicalAddress, size_t *error) {
    size_t *page = (size_t *)_memory->fetch(physicalAddress, _pageSize, 1, error);
    _disk->storeData(virtualAddress, page, _pageSize, error);
    free(page);
}

void PageManager::writePageOnMemory(size_t virtualAddress, size_t physicalAddress, size_t *error) {
    size_t *page = (size_t *)_disk->retrieveData(virtualAddress, _pageSize, 1, error);
    _memory->store(physicalAddress, page, _pageSize, error);
    free(page);
}

void *PageManager::retrieveData(size_t virtualAddress, size_t size, size_t *error) {
    size_t address;
    size_t readCount;
    size_t lastCount = 0;
    size_t *temp;
    size_t *data = (size_t *)calloc(size, sizeof(size_t));
    while (size > 0) {
        address = translateAddress(virtualAddress);
        readCount = (_pageSize-(virtualAddress%_pageSize));
        readCount = (readCount > size ? size : readCount);
        temp = (size_t *)_memory->fetch(address, readCount, 1, error);
        
        *temp = ((*temp) << (lastCount*8));
        *data = ((*data) | (*temp));
        free(temp);
        
        lastCount += readCount;
        size -= readCount;
        virtualAddress += readCount;
    }
    
    #if DEBUG_MODE
    printf("lastAddress:%lu data:%lu\n", virtualAddress, *data);
    #endif
    
    return (void *)data;
}

void PageManager::storeData(size_t virtualAddress, size_t data, size_t size, size_t *error) {
    size_t address;
    size_t writeCount;
    size_t tempData;
    size_t offset;
    while (size > 0) {
        address = translateAddress(virtualAddress);
        writeCount = (_pageSize-(virtualAddress%_pageSize));
        writeCount = (writeCount > size ? size : writeCount);
        tempData = data;
        offset = ((sizeof(size_t)-writeCount)*8);
        tempData = tempData << offset;
        tempData = tempData >> offset;
        _memory->store(address, (void *)&tempData, writeCount, error);
        data = data >> (writeCount*8);
        size -= writeCount;
        virtualAddress += writeCount;
    }
    
}
