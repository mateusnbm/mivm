//
//  MemManager.cpp
//

#include "MemManager.h"

MemManager::MemManager(Memory *memory, Disk *disk, size_t pageSize, size_t *error) {
    if (memory == NULL) {
        *error = kErrorNullPointer;
        #if DEBUG_MODE
        printf("ERRO: MemManager > MemManager() > Memory é NULL.\n");
        #endif
    } else if ( disk == NULL) {
        *error = kErrorNullPointer;
        #if DEBUG_MODE
        printf("ERRO: MemManager > MemManager() > Disk é NULL.\n");
        #endif
    } else {
        
        _memory = memory;
        _disk = disk;
        _pageSize = pageSize;
        
        
        size_t i;
        size_t virtualSlotsCount = _disk->getSize()/_pageSize;
        virtualMap = (Page *)malloc(virtualSlotsCount*sizeof(Page));
        for (i = 0; i < virtualSlotsCount; i++) {
            virtualMap[i].valid = false;
        }
        slotsCount = _memory->getSize()/_pageSize;
        emptySlots = slotsCount;
        physicalMap = (size_t *)calloc(emptySlots, sizeof(size_t));
        for (i = 0; i < emptySlots; i++) {
            physicalMap[i] = -1;
        }
        list = NULL;
        
        
        
        *error = 0;
        
    }
}

void MemManager::writePageOnDisk(size_t virtualAddress, size_t physicalAddress, size_t * error) {
    size_t * page = (size_t *) _memory->fetch(physicalAddress, _pageSize, 1, error);
    _disk->storeData(virtualAddress, page, _pageSize, error);
    free(page);
}

void MemManager::writePageOnMemory(size_t virtualAddress, size_t physicalAddress, size_t * error) {
    size_t * page = (size_t *)_disk->retrieveData(virtualAddress, _pageSize, 1, error);
    _memory->store(physicalAddress, page, _pageSize, error);
    free(page);
}

void * MemManager::retrieveData(size_t virtualAddress, size_t size, size_t * error) {
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

void MemManager::storeData(size_t virtualAddress, size_t data, size_t size, size_t * error) {
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







size_t MemManager::availableIndex() {
    size_t i, k;
    for (i = k = 0; i < slotsCount; i++) {
        if (physicalMap[i] == -1) {
            k = i;
            i = slotsCount;
        }
    }
    return k;
}

Item *MemManager::newItem(size_t virtualIndex, size_t physicalIndex) {
    Item *item = (Item *)malloc(sizeof(Item));
    if (item != NULL) {
        item->virtualIndex = virtualIndex;
        item->physicalIndex = physicalIndex;
        item->next = NULL;
    }
    return item;
}

Item *MemManager::pushItem(Item *list, Item *item) {
    if (list == NULL) {
        list = item;
    }else{
        Item *aux = list;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = item;
    }
    return list;
}

Item *MemManager::popItem(Item **list) {
    Item *aux = *list;
    if (aux != NULL) {
        *list = (*list)->next;
    }
    return aux;
}

void MemManager::newPages(size_t pid, size_t address, size_t pagesCount) {
    
    #if DEBUG_MODE
    printf("pid:%lu address:%lu pagesCount:%lu\n", pid, address, pagesCount);
    #endif
    
    size_t i;
    size_t index = address/_pageSize;
    for (i = index; i < (index+pagesCount); i++) {
        virtualMap[i].pid = pid;
        virtualMap[i].valid = true;
        virtualMap[i].dirty = false;
        virtualMap[i].onMemory = false;
    }
}

size_t MemManager::translateAddress(size_t virtualAddress) {
    size_t error;
    size_t virtualIndex = virtualAddress/_pageSize;
    size_t byteOffset = virtualAddress-(virtualIndex*_pageSize);
    if (virtualMap[virtualIndex].onMemory == false) {
        virtualMap[virtualIndex].onMemory = true;
        if (emptySlots > 0) {
            size_t index = availableIndex();
            virtualMap[virtualIndex].physicalIndex = index;
            physicalMap[index] = virtualIndex;
            writePageOnMemory(virtualIndex*_pageSize, index*_pageSize, &error);
            emptySlots--;
            list = pushItem(list, newItem(virtualIndex, index));
        }else{
            Item *removed = popItem(&list);
            size_t rvi = removed->virtualIndex;
            size_t rpi = removed->physicalIndex;
            virtualMap[virtualIndex].physicalIndex = rpi;
            virtualMap[rvi].onMemory = false;
            if (virtualMap[rvi].valid && virtualMap[rvi].dirty) {
                writePageOnDisk(rvi*_pageSize, rpi*_pageSize, &error);
                virtualMap[rvi].dirty = false;
            }
            writePageOnMemory(virtualIndex*_pageSize, rpi*_pageSize, &error);
            list = pushItem(list, newItem(virtualIndex, rpi));
            free(removed);
        }
    }
    
    #if DEBUG_MODE
    printf("page:%lu at mem:%lu\n", virtualIndex, virtualMap[virtualIndex].physicalIndex);
    #endif
    
    return (virtualMap[virtualIndex].physicalIndex*_pageSize)+byteOffset;
}
