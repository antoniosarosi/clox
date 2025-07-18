#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, obj_type) \
    (type*)allocate_object(sizeof(type), obj_type)

static Obj* allocate_object(size_t size, ObjType type) {
    Obj* object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;
    object->is_marked = false;
    object->next = vm.objects;
    vm.objects = object;

    #ifdef DEBUG_LOG_GC
        printf("%p allocate %zu for %d\n", (void*)object, size, type);
    #endif

    return object;
}

ObjClass* new_class(ObjString* name) {
    ObjClass* cls = ALLOCATE_OBJ(ObjClass, OBJ_CLASS);
    cls->name = name;
    return cls;
}

ObjInstance* new_instance(ObjClass* cls) {
    ObjInstance* instance = ALLOCATE_OBJ(ObjInstance, OBJ_INSTANCE);
    instance->cls = cls;
    init_table(&instance->fields);
    return instance;
}

ObjClosure* new_closure(ObjFunction* function) {
    ObjUpvalue** upvalues = ALLOCATE(ObjUpvalue*, function->upvalue_count);
    for (int i = 0; i < function->upvalue_count; i++) {
        upvalues[i] = NULL;
    }

    ObjClosure* closure = ALLOCATE_OBJ(ObjClosure, OBJ_CLOSURE);
    closure->function = function;
    closure->upvalues = upvalues;
    closure->upvalue_count = function->upvalue_count;
    return closure;
}

ObjFunction* new_function() {
    ObjFunction* function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);
    function->arity = 0;
    function->upvalue_count = 0;
    function->name = NULL;
    init_chunk(&function->chunk);

    return function;
}

ObjNative* new_native(NativeFn function) {
    ObjNative* native = ALLOCATE_OBJ(ObjNative, OBJ_NATIVE);
    native->function = function;
    return native;
}

static ObjString* allocate_string(char* chars, int length, uint32_t hash) {
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;

    push(OBJ_VAL(string));
    table_set(&vm.strings, string, NIL_VAL);
    pop();

    return string;
}

static uint32_t hash_string(const char* key, int length) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }

    return hash;
}

ObjString* take_string(char* chars, int length) {
    uint32_t hash = hash_string(chars, length);

    ObjString* interned = table_find_string(&vm.strings, chars, length, hash);
    if (interned != NULL) {
        FREE_ARRAY(char, chars, length + 1);
        return interned;
    }

    return allocate_string(chars, length, hash);
}

ObjString* copy_string(const char* chars, int length) {
    uint32_t hash = hash_string(chars, length);

    ObjString* interned = table_find_string(&vm.strings, chars, length, hash);
    if (interned != NULL) {
        return interned;
    }

    char* heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocate_string(heapChars, length, hash);
}

ObjUpvalue* new_upvalue(Value* slot) {
    ObjUpvalue* upvalue = ALLOCATE_OBJ(ObjUpvalue, OBJ_UPVALUE);
    upvalue->closed = NIL_VAL;
    upvalue->location = slot;
    upvalue->next = NULL;
    return upvalue;
}

static void print_function(ObjFunction* function) {
    if (function->name == NULL) {
        printf("<script>");
        return;
    }

    printf("<fn %s>",  function->name->chars);
}

void print_object(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_CLASS: {
            printf("<class %s>", AS_CLASS(value)->name->chars);
            break;
        }

        case OBJ_INSTANCE: {
            printf("<%s instance>", AS_INSTANCE(value)->cls->name->chars);
            break;
        }

        case OBJ_STRING: {
            printf("%s", AS_CSTRING(value));
            break;
        }

        case OBJ_UPVALUE: {
            printf("upvalue");
            break;
        }

        case OBJ_FUNCTION: {
            print_function(AS_FUNCTION(value));
            break;
        }

        case OBJ_CLOSURE: {
            print_function(AS_CLOSURE(value)->function);
            break;
        }

        case OBJ_NATIVE: {
            printf("<native fn>");
            break;
        }
    }
}
