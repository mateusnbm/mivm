//
//  Pager.cpp
//  Mom
//
//  Created by Veve on 31/08/14.
//  Copyright (c) 2014 Lits Development. All rights reserved.
//

#include "Pager.h"

size_t Pager::availableIndex() {
    size_t i, k;
    for (i = k = 0; i < slotsCount; i++) {
        if (physicalMap[i] == -1) {
            k = i;
            i = slotsCount;
        }
    }
    return k;
}

Item *Pager::newItem(size_t virtualIndex, size_t physicalIndex) {
    Item *item = (Item *)malloc(sizeof(Item));
    if (item != NULL) {
        item->virtualIndex = virtualIndex;
        item->physicalIndex = physicalIndex;
        item->next = NULL;
    }
    return item;
}

Item *Pager::pushItem(Item *list, Item *item) {
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

Item *Pager::popItem(Item **list) {
    Item *aux = *list;
    if (aux != NULL) {
        *list = (*list)->next;
    }
    return aux;
}

void Pager::init (size_t memorySize, size_t diskSize) {
    size_t i;
    size_t virtualSlotsCount = diskSize/_pageSize;
    virtualMap = (Page *)malloc(virtualSlotsCount*sizeof(Page));
    for (i = 0; i < virtualSlotsCount; i++) {
        virtualMap[i].valid = false;
    }
    slotsCount = memorySize/_pageSize;
    emptySlots = slotsCount;
    physicalMap = (size_t *)calloc(emptySlots, sizeof(size_t));
    for (i = 0; i < emptySlots; i++) {
        physicalMap[i] = -1;
    }
    list = NULL;
}

void Pager::newPages(size_t pid, size_t address, size_t pagesCount) {
    
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

size_t Pager::translateAddress(size_t virtualAddress) {
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