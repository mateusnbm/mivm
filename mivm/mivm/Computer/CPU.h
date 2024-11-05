//
//  CPU.h
//

#ifndef CPU_hpp
#define CPU_hpp

#include <stdio.h>

#include "Defs.h"

#include "RegisterBank.h"
#include "Memory.h"
#include "MemManager.h"

#include "Process.h"

/**
 *
 * @class CPU
 * @brief Representa a Central Processing Unit (CPU) do computador
 *
 * A classe CPU é usada para ...
 *
 */
class CPU {
private:
    RegisterBank * _registers;
    Memory * _memory;
    MemManager * _memManager;
    size_t extendSignal(size_t data, size_t size);
    size_t bitMask (size_t data, size_t higher, size_t less);
public:
    CPU(RegisterBank * registers, Memory * memory, size_t * error);
    void executeInstruction(Process * process, size_t instruction);
    void setMemManager(MemManager * manager);
};

#endif /* CPU_hpp */
