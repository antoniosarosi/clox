#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

/**
 * Instruction set.
 */
typedef enum {
    // TODO: Add OP_CONSTANT_LONG that uses 3 bytes instead of 1 for indexing.
    OP_CONSTANT,
    OP_RETURN,
} OpCode;

/**
 * Bytecode is a series of instructions.
 */
typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    int* lines;
    ValueArray constants;
} Chunk;

void init_chunk(Chunk* chunk);
void free_chunk(Chunk* chunk);
void write_chunk(Chunk* chunk, uint8_t byte, int line);
int add_constant(Chunk* chunk, Value value);

#endif
