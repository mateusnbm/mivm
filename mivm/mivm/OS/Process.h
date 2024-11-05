//
//  Process.h
//

#ifndef Process_hpp
#define Process_hpp

#include <stdio.h>
#include <stdlib.h>

#include "Defs.h"

class Process {
private:
    size_t _programSize;
    size_t _dataSize;
    size_t _baseAddress;
    size_t _ip;
    size_t _type;
    size_t _priority;
public:
    Process(size_t programSize, size_t dataSize, size_t baseAddress, size_t type, size_t priority);
    size_t baseAddress();
    size_t ip();
    bool setIp(size_t newIp);
    size_t type();
    bool active();
    size_t currentAddress();
};

#endif /* Process_hpp */
