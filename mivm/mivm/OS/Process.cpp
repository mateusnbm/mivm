//
//  Process.cpp
//

#include "Process.h"

Process::Process(size_t programSize, size_t dataSize, size_t baseAddress, size_t type, size_t priority) {
    _programSize = programSize;
    _dataSize = dataSize;
    _baseAddress = baseAddress;
    _ip = dataSize;
    _type = type;
    _priority = priority;
}

size_t Process::baseAddress() {
    return _baseAddress;
}

size_t Process::ip() {
    return _ip;
}

bool Process::setIp(size_t newIp) {
    if (newIp <= _programSize) {
        _ip = newIp;
        return true;
    }else{
        return false;
    }
}

size_t Process::type() {
    return _type;
}

bool Process::active() {
    return ((_ip >= _programSize) ? true : false);
}

size_t Process::currentAddress() {
    return _baseAddress+_ip;
}
