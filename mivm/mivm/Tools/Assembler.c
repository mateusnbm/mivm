//
//  Assembler.c
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Defs.h"

#define UNKNOW 0
#define BYTE 1
#define HALFWORD 2
#define WORD 4

typedef struct addressing {
    char *key;
    size_t size;
    size_t address;
    struct addressing *next;
} Addressing;

Addressing *newAddresing (char *key, size_t size, size_t address) {
    Addressing *item = (Addressing *)malloc(sizeof(Addressing));
    if (item != NULL) {
        item->key = key;
        item->size = size;
        item->address = address;
        item->next = NULL;
    }
    return item;
}

Addressing *addAddressing (Addressing *list, Addressing *item) {
    if (list == NULL) {
        list = item;
    }else{
        Addressing *aux = list;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = item;
    }
    return list;
}

size_t addressForKey (Addressing *list, char *key, size_t *error) {
    size_t address = -1;
    while (list != NULL) {
        if (strcmp(list->key, key) == 0) {
            address = list->address;
            list = NULL;
        }else{
            list = list->next;
        }
    }
    if (address == -1) {
        *error = kErrorInvalidKeyName;
    }else{
        *error = 0;
    }
    return address;
}

size_t dataSizeForKey (char *key) {
    size_t size;
    if (strcmp(key, ".byte") == 0) {
        size = BYTE;
    }else if (strcmp(key, ".halfword") == 0) {
        size = HALFWORD;
    }else if (strcmp(key, ".word") == 0) {
        size = WORD;
    }else{
        size = UNKNOW;
    }
    return size;
}

size_t registerFromKey (char *key) {
    size_t reg = (key[1]-'0');
    if (key[2] != ',' && key[2] != 0) {
        reg = ((reg*10)+(key[2]-'0'));
    }
    return reg;
}

size_t composeTypeR (size_t opcode, size_t rs, size_t rt, size_t rd, size_t shamt, size_t funct) {
    size_t instruction = opcode;
    instruction = (instruction << 5);
    instruction = (instruction | rs);
    instruction = (instruction << 5);
    instruction = (instruction | rt);
    instruction = instruction << 5;
    instruction = (instruction | rd);
    instruction = instruction << 5;
    instruction = (instruction | shamt);
    instruction = instruction << 6;
    instruction = (instruction | funct);
    return instruction;
}

size_t composeTypeI (size_t opcode, size_t rs, size_t rt, size_t immediate) {
    //immediate may be negative,so it will come in twos complement format,we need to remove its 16 most significant bytes
    immediate = immediate << ((sizeof(size_t)*8)-16);
    immediate = immediate >> ((sizeof(size_t)*8)-16);
    //
    size_t instruction = opcode;
    instruction = (instruction << 5);
    instruction = (instruction | rs);
    instruction = (instruction << 5);
    instruction = (instruction | rt);
    instruction = instruction << 16;
    instruction = (instruction | immediate);
    return instruction;
}

size_t composeTypeJ (size_t opcode, size_t offset) {
    size_t instruction = opcode;
    instruction = (instruction << 26);
    instruction = (instruction | offset);
    return instruction;
    return 0;
}

void generateAddressing (FILE *file, FILE *machine, Addressing **addressing, size_t *data_size, size_t *error) {
    int read = 1;
    char buffer[120];
    char *data_name;
    char single;
    size_t size;
    int initializer;
    int counter;
    fscanf(file, "%[^\n]", buffer);
    while (read) {
        fscanf(file, "%s", buffer);
        if (strcmp(buffer, "section") == 0) {
            read = 0;
        }else{
            data_name = (char *)malloc(strlen(buffer)*sizeof(char));
            strcpy(data_name, buffer);
            fscanf(file, "%s", buffer);
            size = dataSizeForKey(buffer);
            counter = 0;
            do {
                fscanf(file, "%i%c", &initializer, &single);
                fwrite(&initializer, size, 1, machine);
                counter += size;
            } while (single == ',');
            *addressing = addAddressing(*addressing, newAddresing(data_name, counter, *data_size));
            *data_size += counter;
        }
    }
}

void generateMachineCode (FILE *file, FILE *machine, Addressing *addressing, size_t *program_size, size_t *error) {
    int read = 1;
    char buffer[120];
    char regA[5], regB[5], regC[5];
    char address[12];
    size_t instruction;
    size_t immediate;
    fscanf(file, "%[^\n]", buffer);
    while (read) {
        fscanf(file, "%s", buffer);
        if (feof(file)) {
            read = 0;
        }else{
            if (strcmp(buffer, "lb") == 0) {
                fscanf(file, "%s %s", regA, address);
                instruction = composeTypeI(LB_OPCODE, registerFromKey(regA), 0, addressForKey(addressing, address, error));
            }else if (strcmp(buffer, "lh") == 0) {
                fscanf(file, "%s %s", regA, address);
                instruction = composeTypeI(LH_OPCODE, registerFromKey(regA), 0, addressForKey(addressing, address, error));
            }else if (strcmp(buffer, "lw") == 0) {
                fscanf(file, "%s %s", regA, address);
                instruction = composeTypeI(LW_OPCODE, registerFromKey(regA), 0, addressForKey(addressing, address, error));
            }else if (strcmp(buffer, "sb") == 0) {
                fscanf(file, "%s %s", regA, address);
                instruction = composeTypeI(SB_OPCODE, registerFromKey(regA), 0, addressForKey(addressing, address, error));
            }else if (strcmp(buffer, "sh") == 0) {
                fscanf(file, "%s %s", regA, address);
                instruction = composeTypeI(SH_OPCODE, registerFromKey(regA), 0, addressForKey(addressing, address, error));
            }else if (strcmp(buffer, "sw") == 0) {
                fscanf(file, "%s %s", regA, address);
                instruction = composeTypeI(SW_OPCODE, registerFromKey(regA), 0, addressForKey(addressing, address, error));
            }else if (strcmp(buffer, "add") == 0) {
                fscanf(file, "%s %s %s", regA, regB, regC);
                instruction = composeTypeR(0x0, registerFromKey(regA), registerFromKey(regB), registerFromKey(regC), 0x0, ADD_FUNCT);
            }else if (strcmp(buffer, "sub") == 0) {
                fscanf(file, "%s %s %s", regA, regB, regC);
                instruction = composeTypeR(0x0, registerFromKey(regA), registerFromKey(regB), registerFromKey(regC), 0x0, SUB_FUNCT);
            }else if (strcmp(buffer, "beq") == 0) {
                fscanf(file, "%s %s %lu", regA, regB, &immediate);
                instruction = composeTypeI(BEQ_OPCODE, registerFromKey(regA), registerFromKey(regB), immediate);
            }else if (strcmp(buffer, "bne") == 0) {
                fscanf(file, "%s %s %lu", regA, regB, &immediate);
                instruction = composeTypeI(BNE_OPCODE, registerFromKey(regA), registerFromKey(regB), immediate);
            }else if (strcmp(buffer, "ptr_asciiz") == 0) {
                fscanf(file, "%s", address);
                instruction = composeTypeJ(PTR_ASCIIZ_OPCODE, addressForKey(addressing, address, error));
            }else if (strcmp(buffer, "ptr_int") == 0) {
                fscanf(file, "%s %s", regA, address);
                instruction = composeTypeI(PTR_INT_OPCODE, (regA[0]-'0'), 0, addressForKey(addressing, address, error));
            }
            fwrite(&instruction, INSTRUCTION_SIZE, 1, machine);
            *program_size += INSTRUCTION_SIZE;
        }
    }
}

FILE * assemble(FILE * code, size_t * program_size, size_t * data_size, size_t * error) {
    FILE * machine = NULL;
    Addressing * addressing = NULL;
    if (code == NULL) {
        *error = kErrorInvalidFileStream;
    }else{
        *program_size = 0;
        *data_size = 0;
        machine = fopen("machine.bin", "wb+");
        generateAddressing(code, machine, &addressing, data_size, error);
        generateMachineCode(code, machine, addressing, program_size, error);
        *program_size += *data_size;
    }
    return machine;
}
