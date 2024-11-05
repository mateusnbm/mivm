//
//  OS.hpp
//

#ifndef OS_hpp
#define OS_hpp

#include <iostream>

#include "Defs.h"
#include "Params.h"

#include "Hardware.h"

#include "MemManager.h"
#include "Program.h"
#include "Process.h"

#include "Scheduler.h"
#include "PageReplacer.h"

typedef struct processItem {
    size_t pid;
    size_t * registers;
    Process * process;
    struct processItem *next;
} ProcessItem;

/**
 *
 * @class OS
 * @brief Representa sistema operacional no computador.
 *
 * A classe OS é usada para simular o funcionamento do sistema operacional no computador.
 * Ela mantém a lista dos processos que deverão ser executados no computador.
 * Coordena o escalonamento dos processos.
 * Coordena a paginação.
 *
 */
class OS {
private:
    size_t _pidBase;
    size_t _pidCount;
    Hardware * _hardware;
    MemManager * _memManager;
    Scheduler * _scheduler;
    PageReplacer * _replacer;
    ProcessItem * _processList;
    ProcessItem * createProcessItem (size_t pid, Process * process);
    ProcessItem * addProcessItem(ProcessItem * list, ProcessItem * item);
    ProcessItem * findProcessItem(ProcessItem * list, size_t pid);
    ProcessItem * removeProcessItem(ProcessItem * list, size_t pid);
public:
    OS(Hardware * hardware, Scheduler * scheduler, PageReplacer * replacer, size_t * error);
    size_t createProcess(Program * program, size_t type, size_t priority, size_t * error);
    void execute(size_t * error);
};

#endif /* OS_hpp */
