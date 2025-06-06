#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "table.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    Chunk* chunk;
    // Instruction Pointer (IP) or Program Counter (PC)
    uint8_t* ip;
    Value stack[STACK_MAX];
    // Points to the next free slot in the stack.
    Value* stack_top;
    Table strings;
    Obj* objects;
} Vm;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

extern Vm vm;

void init_vm();
void free_vm();
InterpretResult interpret(const char* source);

void push(Value value);
Value pop();

#endif
