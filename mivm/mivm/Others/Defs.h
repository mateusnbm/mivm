//
//  Defs.h
//

#ifndef Defs_h
#define Defs_h

// macros
#define APPATH(x) "/Users/mateusnbm/Desktop/mivm/mivm/mivm/Applications/" x

// instructions
#define INSTRUCTION_SIZE 0x4
#define LB_OPCODE 0x20
#define LH_OPCODE 0x21
#define LW_OPCODE 0x23
#define SB_OPCODE 0x28
#define SH_OPCODE 0x29
#define SW_OPCODE 0x2B
#define ADD_FUNCT 0x20
#define SUB_FUNCT 0x22
#define BEQ_OPCODE 0x4
#define BNE_OPCODE 0x5
#define PTR_ASCIIZ_OPCODE 0x30
#define PTR_INT_OPCODE 0x31

// errors
#define kErrorMemoryAlloc 1
#define kErrorInvalidFileStream 2
#define kErrorFileOpen 3
#define kErrorFileSeek 4
#define kErrorFileWrite 5
#define kErrorFileRead 6
#define kErrorFileClose 7
#define kErrorInvalidAddress 8
#define kErrorSpaceUnavailable 9
#define kErrorNullPointer 10
#define kInvalidFileStream 11
#define kProcessFinished 12
#define kErrorInvalidKeyName 13

// process priorities
#define kPriorityHigh 10
#define kPriorityMedium 5
#define kPriorityLow 1

// process types
#define kTypeDaemon 1
#define kTypeAutomatic 2
#define kTypeInteractive 3

#endif /* Defs_h */
