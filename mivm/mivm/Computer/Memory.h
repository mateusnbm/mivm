//
//  Memory.h
//

#ifndef Memory_hpp
#define Memory_hpp

#include <stdio.h>
#include <stdlib.h>

#include "Defs.h"

/**
 *
 * @class Memory
 * @brief Representa a memória principal do computador.
 *
 * A classe Memory é usada para simular a memória principal do computador.
 * Ela cria um arquivo binário, totalmente zerado, com o tamanho especificado.
 * Possibilita escritas e leituras.
 *
 */
class Memory {
private:
    size_t _size;
    FILE * file;
public:
    Memory(size_t size, size_t * error);
    ~Memory();
    void store(size_t address, void * data, size_t size, size_t * error);
    void * fetch(size_t address, size_t size, size_t count, size_t * error);
    size_t getSize();
};

#endif /* Memory_hpp */
