//
//  CPU.cpp
//

#include "CPU.h"

size_t CPU::extendSignal(size_t data, size_t size) {
    size_t bit = data >> ((size*8)-1);
    if (bit) {
        size_t temp = 0;
        temp--;
        temp = (temp << (8*size));
        temp = (temp | data);
        data = temp;
    }
    return data;
}

size_t CPU::bitMask (size_t data, size_t higher, size_t less) {
    size_t offset = ((sizeof(size_t)*8)-(higher+1));
    data = (data << offset);
    data = (data >> (offset+less));
    return data;
}

/**
 * @brief Cria uma nova CPU com acesso aos registradores e a memória principal do computador.
 *
 * @param registers Os registradores do computador.
 * @param memory A memória principal do computador.
 * @param error Referência utilizada para retornar o código de um possível erro.
 *
 */
CPU::CPU (RegisterBank * registers, Memory * memory, size_t * error) {
    if (registers == NULL) {
        *error = kErrorNullPointer;
        #if DEBUG_MODE
        printf("ERRO: CPU > CPU() > Registers é NULL");
        #endif
    } else if (memory == NULL) {
        *error = kErrorNullPointer;
        #if DEBUG_MODE
        printf("ERRO: CPU > CPU() > Memory é NULL");
        #endif
    } else {
        _registers = registers;
        _memory = memory;
        *error = 0;
    }
}

void CPU::executeInstruction(Process * process, size_t instruction) {
    size_t aux;
    size_t error = 0;
    size_t base_address = process->baseAddress();
    size_t *data;
    size_t address;
    size_t opcode = bitMask(instruction, 31, 26);
    size_t regA = bitMask(instruction, 25, 21);
    size_t regB = bitMask(instruction, 20, 16);
    size_t regC = bitMask(instruction, 15, 11);
    size_t funct = bitMask(instruction, 5, 0);
    size_t immediate = extendSignal(bitMask(instruction, 15, 0), 2);
    
    #if DEBUG_MODE
    printf("opcode:%lu\n", opcode);
    #endif
    
    switch (opcode) {
        case 0x0:
            switch (funct) {
                case ADD_FUNCT:
                    aux = _registers->retrieveData(regB)+_registers->retrieveData(regC);
                    _registers->storeData(regA, &aux);
                    break;
                case SUB_FUNCT:
                    aux = _registers->retrieveData(regB)-_registers->retrieveData(regC);
                    _registers->storeData(regA, &aux);
                    break;
            }
            break;
        case LB_OPCODE:
            data = (size_t *)_memManager->retrieveData(immediate+base_address, 1, &error);
            *data = extendSignal(*data, 1);
            _registers->storeData(regA, (size_t *)data);
            break;
        case LH_OPCODE:
            data = (size_t *)_memManager->retrieveData(immediate+base_address, 2, &error);
            *data = extendSignal(*data, 2);
            _registers->storeData(regA, (size_t *)data);
            break;
        case LW_OPCODE:
            data = (size_t *)_memManager->retrieveData(immediate+base_address, 4, &error);
            *data = extendSignal(*data, 4);
            _registers->storeData(regA, (size_t *)data);
            break;
        case SB_OPCODE:
            aux = _registers->retrieveData(regA);
            _memManager->storeData(immediate+base_address, aux, 1, &error);
            break;
        case SH_OPCODE:
            aux = _registers->retrieveData(regA);
            _memManager->storeData(immediate+base_address, aux, 2, &error);
            break;
        case SW_OPCODE:
            aux = _registers->retrieveData(regA);
            _memManager->storeData(immediate+base_address, aux, 4, &error);
            break;
        case BEQ_OPCODE:
            if (_registers->retrieveData(regA) == _registers->retrieveData(regB)) {
                int imm = (int)immediate*4;
                if (imm < 0) {
                    imm -= 4;
                }
                process->setIp(process->ip()+imm);
            }
            break;
        case BNE_OPCODE:
            if (_registers->retrieveData(regA) != _registers->retrieveData(regB)) {
                int imm = (int)immediate*4;
                if (imm < 0) {
                    imm -= 4;
                }
                process->setIp(process->ip()+imm);
            }
            break;
        case PTR_ASCIIZ_OPCODE:
            aux = immediate+base_address;
            do {
                address = _memManager->translateAddress(aux);
                data = (size_t *)_memory->fetch(address, 1, 1, &error);
                if (*data != 0) {
                    printf("%c", (char)*data);
                    aux++;
                }
            } while (*data != 0);
            break;
        case PTR_INT_OPCODE:
            data = (size_t *)_memManager->retrieveData(immediate+base_address, regA, &error);
            printf("%lu\n", *data);
            break;
    }
}

void CPU::setMemManager(MemManager * manager) {
    _memManager = manager;
}
