//
//  Disk.h
//

#ifndef Disk_hpp
#define Disk_hpp

#include <stdio.h>
#include <stdlib.h>

#include "Defs.h"

typedef struct space {
    bool occupied;
    size_t address;
    size_t size;
    struct space * next;
    
} Space;

/**
 *
 * @class Disk
 * @brief Representa o armazenamento secundário do computador.
 *
 * A classe Disk é usada para simular o armazenamento secundário do computador.
 * Ela cria um arquivo binário, totalmente zerado, com o tamanho especificado.
 * Possibilita escritas e leituras.
 * Faz o gerenciamento dos blocos... (deveria?)
 *
 */
class Disk {
private:
    size_t _size;
    FILE * file;
    Space * mapping;
    Space * newSpace(bool occupied, size_t address, size_t size);
public:
    Disk(size_t size, size_t *error);
    ~Disk();
    void storeData(size_t address, void * data, size_t size, size_t * error);
    void * retrieveData(size_t address, size_t size, size_t count, size_t * error);
    void storeFileContent(size_t address, FILE * data, size_t size, size_t * error);
    size_t allocateMemory(size_t size, size_t * error);
    void freeMemory(size_t address, size_t * error);
    size_t getSize();
};

#endif /* Disk_hpp */
