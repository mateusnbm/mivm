//
//  Program.cpp
//

#include "Program.h"

#include "Assembler.c"

Program::Program(FILE * source, size_t * error) {
    if (source == NULL) {
        *error = kErrorInvalidFileStream;
    } else {
        binaries = assemble(source, &program_size, &data_size, error);
        if (!error) {
            // ok
        }else{
            // report errors
        }
    }
}

Program::~Program() {
    fclose(binaries);
}
