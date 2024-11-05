//
//  Hardware.hpp
//

#ifndef Hardware_hpp
#define Hardware_hpp

#include <stdio.h>

#include "CPU.h"
#include "RegisterBank.h"
#include "Memory.h"
#include "Disk.h"

class Hardware {
private:
    RegisterBank * _registers;
    Memory * _memory;
    Disk * _disk;
    CPU * _cpu;
public:
    Hardware(CPU * cpu, RegisterBank * registers, Memory * memory, Disk * disk);
    CPU * getCPU();
    RegisterBank * getRegisters();
    Memory * getMemory();
    Disk * getDisk();
};

#endif /* Hardware_hpp */
