//
//  OS.cpp
//

#include "OS.h"

ProcessItem * OS::createProcessItem (size_t pid, Process * process) {
    ProcessItem * item = (processItem *) malloc(sizeof(ProcessItem));
    if (item != NULL) {
        item->pid = pid;
        item->process = process;
        item->next = NULL;
    }
    return item;
}

ProcessItem * OS::addProcessItem(ProcessItem * list, ProcessItem * item) {
    if (list == NULL) {
        list = item;
    } else {
        ProcessItem * aux = list;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = item;
    }
    return list;
}

ProcessItem * OS::findProcessItem(ProcessItem * list, size_t pid) {
    if (list) {
        while (list->pid != pid) {
            list = list->next;
        }
    }
    return list;
}

ProcessItem * OS::removeProcessItem(ProcessItem * list, size_t pid) {
    if (list) {
        if (list->pid == pid) {
            list = list->next;
        } else {
            ProcessItem * aux = list;
            while (aux) {
                if (aux->next && aux->next->pid == pid) {
                    ProcessItem * next = aux->next->next;
                    aux->next->next = NULL;
                    aux->next = next;
                } else {
                    aux = aux->next;
                }
            }
            
        }
    }
    return list;
}

/**
 * @brief Constrói um novo OS com os parâmetros passados.
 *
 * Guarda referências para o hardware, scheduler e pageManager.
 * Inicializa as demais variáveis da classe.
 *
 * @param hardware O hardware onde serão executados os programas.
 * @param scheduler O algoritmo de escalonamento de processos.
 * @param replacer O algoritmo de substituição de páginas.
 * @param error  Referência utilizada para retornar o código de um possível erro.
 */
OS::OS(Hardware * hardware, Scheduler * scheduler, PageReplacer * replacer, size_t * error) {
    if (hardware == NULL) {
        *error = kErrorNullPointer;
        #if DEBUG_MODE
        printf("ERRO: OS > OS() > Hardware é NULL.\n");
        #endif
    } else if (scheduler == NULL) {
        *error = kErrorNullPointer;
        #if DEBUG_MODE
        printf("ERRO: OS > OS() > Scheduler é NULL.\n");
        #endif
    } else if (replacer == NULL) {
        *error = kErrorNullPointer;
        #if DEBUG_MODE
        printf("ERRO: OS > OS() > Replacer é NULL.\n");
        #endif
    } else {
        
        _pidBase = (100 + ( std::rand() % ( 1000 - 100 + 1 ) ));
        _pidCount = 0;
        _hardware = hardware;
        _scheduler = scheduler;
        _replacer = replacer;
        _processList = NULL;
        
        _memManager = new MemManager(_hardware->getMemory(), _hardware->getDisk(), PAGE_SIZE, error);
        
        _hardware->getCPU()->setMemManager(_memManager);
        
        *error = 0;
        
    }
}

/**
 * @brief Cria e registra um novo processo perante o sistema operacional.
 *
 * Reserva o espaço necessário no disco.
 * Escreve os binários do programa (código compilado) no disco.
 * Cria o processo.
 * Adiciona o processo a fila de execução.
 *
 * @param program O programa compilado.
 * @param type O tipo do processo (pode ser usado no escalonamento).
 * @param priority A prioridade do processo (pode ser usado no escalonamento).
 * @param error Referência utilizada para retornar o código de um possível erro.
 *
 * @return Retorna PID do processo.
 */
size_t OS::createProcess(Program * program, size_t type, size_t priority, size_t * error) {
    
    *error = 0;
    
    size_t pid = (_pidBase + _pidCount++);
    size_t qntPages = ceil(program->program_size / PAGE_SIZE);
    size_t address = _hardware->getDisk()->allocateMemory((qntPages * PAGE_SIZE), error);
    
    if (*error == 0) {
        
        _hardware->getDisk()->storeFileContent(address, program->binaries, program->program_size, error);
        
        if (*error == 0) {
            
            Process * process = new Process(program->program_size, program->data_size, address, type, priority);
            
            ProcessItem * item = createProcessItem(pid, process);
            
            if (process && item) {
                
                _processList = addProcessItem(_processList, item);
                
                _scheduler->addProcess(pid);
                
            }
            
        }
        
    }
    
    return pid;
    
}

void OS::execute(size_t * error) {
    
    size_t i;
    size_t instruction;
    
    size_t nextPID = -1;
    size_t currentPID = -1;
    
    ProcessItem * currentItem = NULL;
    Process * currentProcess = NULL;
    
    ProcessItem * nextItem = NULL;
    Process * nextProcess = NULL;
    
    while (_processList != NULL) {
        
        nextPID = _scheduler->switchContext();
        nextItem = findProcessItem(_processList, nextPID);
        nextProcess = nextItem->process;
        
        #if DEBUG_MODE
        printf("OS > execute() > Escalonador escolheu: %lu\n", nextPID);
        #endif
        
        if (currentPID != nextPID) {
            if (currentItem != NULL) {
                if (currentItem->registers != NULL) {
                    free(currentItem->registers); // libera cópia antiga
                }
                currentItem->registers = _hardware->getRegisters()->packValues(); // salva valores atuais
            }
            currentPID = nextPID;
            currentItem = nextItem;
            currentProcess = nextProcess;
            if (currentItem->registers == NULL) {
                _hardware->getRegisters()->resetRegisters();
            } else {
                _hardware->getRegisters()->replaceValues(currentItem->registers);
            }
        }
        
        for (i = 0; i < SCHEDULER_TICK; i++) {
            
            if (currentProcess->active()) {
                
                _processList = removeProcessItem(_processList, currentPID);
                
                _scheduler->removeProcess(currentPID);
                
                i = SCHEDULER_TICK;
                
            }else{
                
                instruction = *((size_t *)_memManager->retrieveData(currentProcess->currentAddress(), INSTRUCTION_SIZE, error));
                
                if (*error == 0) {
                    currentProcess->setIp(currentProcess->ip()+4);
                    _hardware->getCPU()->executeInstruction(currentProcess, instruction);
                }else{
                    switch (*error) {
                        case kErrorInvalidFileStream:
                            printf("Error:Invalid file stream.\n");
                            break;
                        case kErrorFileSeek:
                            printf("Error:Failed to seek on the file.\n");
                            break;
                        case kErrorFileRead:
                            printf("Error:Failed to read data from file.\n");
                            break;
                    }
                }
                
            }
            
        }
        
    }
}
