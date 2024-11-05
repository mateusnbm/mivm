//
//  Memory.cpp
//

#include "Memory.h"

Memory::Memory(size_t size, size_t * error) {
    file = fopen("memory.bin", "wb+");
    if (file == NULL) {
        *error = kErrorFileOpen;
    } else {
        size_t i;
        *error = 0;
        for (i = 0; i < size; i++) {
            if (fwrite("", 1, 1, file) != 1) {
                i = size;
                *error = kErrorFileWrite;
            }
        }
        _size = size;
    }
}

Memory::~Memory() {
    if (file != NULL) {
        if (!fclose(file)) {
            file = NULL;
        }
    }
}

void Memory::store(size_t address, void * data, size_t size, size_t * error) {
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

void * Memory::fetch (size_t address, size_t size, size_t count, size_t * error) {
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
    }
    return (void *)block;
}

size_t Memory::getSize() {
    return _size;
}
