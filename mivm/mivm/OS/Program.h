//
//  Program.h
//

#ifndef Program_hpp
#define Program_hpp

#include <stdio.h>

class Program {
private:
public:
    FILE * binaries;
    size_t program_size;
    size_t data_size;
    Program(FILE * source, size_t * error);
    ~Program();
};

#endif
