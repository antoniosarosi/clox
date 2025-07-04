#ifndef clox_vm_h
#define clox_vm_h

#include "object.h"
#include "table.h"
#include "value.h"

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

// A call frame represents a single ongoing function call.
typedef struct {
    ObjClosure* closure;
    uint8_t* ip;
    Value* slots;
} CallFrame;

typedef struct {
    CallFrame frames[FRAMES_MAX];
    int frame_count;
    Value stack[STACK_MAX];
    // Points to the next free slot in the stack.
    Value* stack_top;
    Table globals;
    Table strings;
    ObjUpvalue* open_upvalues;
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
