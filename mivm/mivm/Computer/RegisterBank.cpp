//
//  RegisterBank.cpp
//

#include "RegisterBank.h"

RegisterBank::RegisterBank(size_t size, size_t * error) {
    registers = (size_t *) calloc(size, sizeof(size_t));
    if (registers == NULL) {
        *error = kErrorMemoryAlloc;
    }else{
        _size = size;
        *error = 0;
    }
}

RegisterBank::~RegisterBank() {
    free(registers);
}

void RegisterBank::storeData(size_t reg, size_t * data) {
    registers[reg] = *data;
}

size_t RegisterBank::retrieveData(size_t reg) {
    return registers[reg];
}

size_t *RegisterBank::packValues() {
    size_t i;
    size_t *temp = (size_t *) malloc(_size * sizeof(size_t));
    for (i = 0; i < _size; i++) {
        temp[i] = registers[i];
    }
    return temp;
}

void RegisterBank::replaceValues(size_t * values) {
    size_t i;
    for (i = 0; i < _size; i++) {
        registers[i] = values[i];
    }
}

void RegisterBank::resetRegisters() {
    size_t i;
    for (i = 0; i < _size; i++) {
        registers[i] = 0;
    }
}
