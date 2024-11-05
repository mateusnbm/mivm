//
//  Scheduler.h
//

#ifndef Scheduler_hpp
#define Scheduler_hpp

#include <stdio.h>

class Scheduler {
private:
    size_t _current;
    size_t _qnt;
    size_t _list[100];
public:
    bool isPreemptive;
    Scheduler();
    void addProcess(size_t pid);
    void removeProcess(size_t pid);
    size_t switchContext();
};

#endif /* Scheduler_hpp */
