//
//  Disk.cpp
//

#include "Disk.h"

Space * Disk::newSpace(bool occupied, size_t address, size_t size) {
    Space * nw = (Space *) malloc(sizeof(Space));
    if (nw != NULL) {
        nw->occupied = occupied;
        nw->address = address;
        nw->size = size;
        nw->next = NULL;
    }
    return nw;
}

Disk::Disk(size_t size, size_t * error) {
    file = fopen("disk.bin", "wb+");
    if (file == NULL) {
        *error = kErrorFileOpen;
    }else{
        size_t i;
        *error = 0;
        Space * first = newSpace(false, 0, size);
        if (first != NULL) {
            _size = size;
            for (i = 0; i < size; i++) {
                if (fwrite("", 1, 1, file) != 1) {
                    i = size;
                    *error = kErrorFileWrite;
                }
            }
            mapping = first;
        }else{
            *error = kErrorMemoryAlloc;
        }
    }
}

Disk::~Disk() {
    if (file != NULL) {
        if (!fclose(file)) {
            file = NULL;
        }
    }
    if (mapping != NULL) {
        Space * aux;
        while (mapping != NULL) {
            aux = mapping;
            mapping = mapping->next;
            free(aux);
        }
    }
}

void Disk::storeData(size_t address, void * data, size_t size, size_t * error) {
    if (file != NULL) {
        if (!fseek(file, address, SEEK_SET)) {
            if (fwrite(data, size, 1, file) == 1) {
                *error = 0;
            }else{
                *error = kErrorFileWrite;
            }
        }else{
            *error = kErrorFileSeek;
        }
    }else{
        *error = kErrorInvalidFileStream;
    }
}

void *Disk::retrieveData(size_t address, size_t size, size_t count, size_t * error) {
    size_t * block;
    if (file != NULL) {
        block = (size_t *) calloc(count,sizeof(size_t));
        if (!fseek(file, address, SEEK_SET)) {
            if (fread(block, size, count, file) == count) {
                *error = 0;
            }else{
                *error = kErrorFileRead;
            }
        }else{
            *error = kErrorFileSeek;
        }
    }else{
        block = NULL;
        *error = kErrorInvalidFileStream;
    }
    return (void *)block;
}

void Disk::storeFileContent(size_t address, FILE * data, size_t size, size_t * error) {
    if (file != NULL) {
        size_t buffer = 0;
        size += address;
        if (!fseek(data, 0, SEEK_SET)) {
            *error = 0;
            while (address < size) {
                if (fread(&buffer, 1, 1, data) == 1) {
                    storeData(address, &buffer, 1, error);
                    address++;
                    if (*error) {
                        address = size;
                    }
                }else{
                    *error = kErrorFileRead;
                }
            }
        }else{
            *error = kErrorFileSeek;
        }
    }else{
        *error = kErrorInvalidFileStream;
    }
}

size_t Disk::allocateMemory(size_t size, size_t * error) {
    size_t address = 0;
    Space * aux = mapping;
    while (aux != NULL) {
        if (aux->occupied == false && aux->size >= size) {
            if (aux->size == size) {
                aux->occupied = true;
                address = aux->address;
            }else{
                address = aux->address+size;
                Space *split = newSpace(false, address, (aux->size-size));
                if (split != NULL) {
                    split->next = aux->next;
                    aux->next = split;
                    aux->occupied = true;
                    aux->size = size;
                    *error = 0;
                }else{
                    *error = kErrorMemoryAlloc;
                }
                address = aux->address;
            }
            aux = NULL;
        }else{
            if (aux->next == NULL) {
                *error = kErrorSpaceUnavailable;
            }
            aux = aux->next;
        }
    }
    return address;
}

void Disk::freeMemory(size_t address, size_t * error) {
    if (address == 0) {
        if (mapping->next != NULL) {
            if (mapping->next->occupied == false) {
                mapping->next->size += mapping->size;
                Space *rem = mapping;
                mapping = mapping->next;
                free(rem);
            }else{
                mapping->occupied = false;
            }
        }else{
            mapping->occupied = false;
        }
        *error = 0;
    }else{
        Space * aux = mapping;
        while (aux->next != NULL) {
            if (aux->next->address == address) {
                Space *after = aux->next->next;
                if (aux->occupied == false) {
                    if (after->occupied == false) {
                        aux->size += (aux->next->size+after->size);
                        free(aux->next);
                        aux->next = after->next;
                        free(after);
                    }else{
                        aux->size += aux->next->size;
                        free(aux->next);
                        aux->next = after;
                    }
                }else{
                    if (after->occupied == false) {
                        aux = aux->next;
                        aux->size += after->size;
                        aux->next = after->next;
                        free(after);
                    }else{
                        aux->next->occupied = false;
                    }
                }
                aux = NULL;
                *error = 0;
            }else{
                aux = aux->next;
                if (aux->next == NULL) {
                    *error = kErrorInvalidAddress;
                }
            }
        }
    }
}

size_t Disk::getSize() {
    return _size;
}
