//
//  RegisterBank.h
//

#ifndef RegisterBank_hpp
#define RegisterBank_hpp

#include <stdio.h>
#include <stdlib.h>

#include "Defs.h"

/**
 *
 * @class RegisterBank
 * @brief Representa o banco de registradores do computador.
 *
 * A classe RegisterBank simula o banco de registradores do computador.
 *
 */
class RegisterBank {
private:
    size_t _size;
    size_t * registers;
public:
    RegisterBank(size_t size, size_t * error);
    ~RegisterBank();
    void storeData(size_t reg, size_t * data);
    size_t retrieveData(size_t reg);
    size_t * packValues();
    void replaceValues(size_t * values);
    void resetRegisters();
};

#endif /* RegisterBank_hpp */
