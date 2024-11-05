//
//  main.cpp
//

#include <iostream>

#include "Defs.h"

#include "Hardware.h"
#include "CPU.h"
#include "RegisterBank.h"
#include "Memory.h"
#include "Disk.h"

#include "OS.h"
#include "Program.h"
#include "Process.h"

#include "Scheduler.h"
#include "PageReplacer.h"

int main(int argc, const char * argv[]) {
    
    size_t error;
    
    // cria o hardware do computador
    
    Disk * disk = new Disk(DISK_SIZE, &error);
    Memory * memory = new Memory(MEMORY_SIZE, &error);
    RegisterBank * registers = new RegisterBank(REGISTER_BANK_COUNT, &error);
    
    CPU * cpu = new CPU(registers, memory, &error);
    
    Hardware * hardware = new Hardware(cpu, registers, memory, disk);
    
    printf("Error1: %li\n", error);
    
    // cria o sistema operacional
    
    Scheduler * scheduler = new Scheduler();
    PageReplacer * replacer = new PageReplacer();
    
    OS * os  = new OS(hardware, scheduler, replacer, &error);
    
    // cria o programa
    
    Program * program = new Program(fopen(APPATH("sum.mips"), "rw"), &error);
    
    printf("Error2: %li\n", error);
    
    // cria o processo
    
    os->createProcess(program, kTypeDaemon, kPriorityLow, &error);
    
    printf("Error3: %li\n", error);
    
    // executa os processos
    
    os->execute(&error);
    
    
    
    
    
    
    
    
    
    // printf("Error: %li\n", error);
    
    delete program;
    
    return 0;
    
//    
//    RegisterBank * bank = new RegisterBank(REGISTER_BANK_COUNT, &error);
//    Memory * memory = new Memory(MEMORY_SIZE, &error);
//    Disk * disk = new Disk(DISK_SIZE, &error);
//    
//    SchedulerRand * rand = new SchedulerRand();
//    Pager * pager = new Pager(memory, disk, PAGE_SIZE, &error);
//    pager->init(MEMORY_SIZE, DISK_SIZE);
//    
//    CPU * cpu = new CPU(bank, memory, pager, &error);
//    Executer * executer = new Executer(bank, cpu, rand, pager, &error);
//    
//    //user requests to load a program, the program is assembled and inserted on a waiting pool
//    Pool *assembled = new Pool();
//    Program *programA = new Program(fopen("branches.mips", "rw"), DAEMON, LOW, &error);
//    assembled->addObject(programA, &error);
//    Program *programB = new Program(fopen("data.mips", "rw"), DAEMON, LOW, &error);
//    assembled->addObject(programB, &error);
//    Program *programC = new Program(fopen("sum.mips", "rw"), DAEMON, LOW, &error);
//    assembled->addObject(programC, &error);
//    Program *programD = new Program(fopen("loop.mips", "rw"), DAEMON, LOW, &error);
//    assembled->addObject(programD, &error);
//    
//    //user requests to add a program to the execution pool
//    size_t pagesCount;
//    size_t address;
//    size_t pids = 0;
//    Program *prog = (Program *)assembled->removeObject();
//    while (prog != NULL) {
//        pagesCount = (prog->program_size/PAGE_SIZE)+(((prog->program_size%PAGE_SIZE)+(PAGE_SIZE-1))/PAGE_SIZE);
//        address = disk->allocateMemory(pagesCount*PAGE_SIZE, &error);
//        if (!error) {
//            disk->storeFileContent(address, prog->binaries, prog->program_size, &error);
//            if (!error) {
//                pager->newPages(pids, address, pagesCount);
//                Process *proc = new Process(prog->program_size, prog->data_size, address, prog->_type, prog->_priority, &error);
//                executer->addProcess(proc, &error);
//            }else{
//                switch (error) {
//                    case kErrorInvalidFileStream:
//                        printf("Error:Invalid file stream.\n");
//                        break;
//                    case kErrorFileSeek:
//                        printf("Error:Failed to seek on the file.\n");
//                        break;
//                    case kErrorFileRead:
//                        printf("Error:Faile to read data from file.\n");
//                        break;
//                    case kErrorFileWrite:
//                        printf("Error:Failed to write data on disk.\n");
//                        break;
//                }
//            }
//        }else{
//            switch (error) {
//                case kErrorMemoryAlloc:
//                    printf("Error:Failed to allocate memory on disk.\n");
//                    break;
//                case kErrorSpaceUnavailable:
//                    printf("Error:Unavailable space on disk.\n");
//                    break;
//            }
//        }
//        pids++;
//        prog = (Program *)assembled->removeObject();
//    }
//    
//    //user requests to execute the selected programs
//    executer->execute(&error);
//    
//    //release memory
//    delete bank;
//    delete memory;
//    delete disk;
//    delete rand;
//    delete pager;
//    delete cpu;
//    delete executer;
//    delete programA;
//    delete programB;
//    delete programC;
//    delete programD;
    
    
    
    
    
    std::cout << "Hello, World!\n";
    
    return 0;
    
}
