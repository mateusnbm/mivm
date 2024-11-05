//
//  Scheduler.cpp
//

#include "Scheduler.h"

Scheduler::Scheduler() {
    _current = 0;
    _qnt = 0;
    isPreemptive = true;
}

void Scheduler::addProcess(size_t pid) {
    _list[_qnt++] = pid;
}

void Scheduler::removeProcess(size_t pid) {
    _current++;
}

size_t Scheduler::switchContext() {
    return ( _qnt > _current ? _list[_current] : -1 );
}
