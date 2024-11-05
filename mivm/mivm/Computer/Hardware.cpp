//
//  Hardware.cpp
//

#include "Hardware.h"

Hardware::Hardware(CPU * cpu, RegisterBank * registers, Memory * memory, Disk * disk) {
    _cpu = cpu;
    _registers = registers;
    _memory = memory;
    _disk = disk;
}

CPU * Hardware::getCPU() {
    return _cpu;
}

RegisterBank * Hardware::getRegisters() {
    return _registers;
}

Memory * Hardware::getMemory() {
    return _memory;
}

Disk * Hardware::getDisk() {
    return _disk;
}
