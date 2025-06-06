#include <stdlib.h>

#include "memory.h"
#include "vm.h"

void* reallocate(void* pointer, size_t old_size, size_t new_size) {
    if (new_size == 0) {
        free(pointer);
        return NULL;
    }

    void* new_pointer = realloc(pointer, new_size);

    // Out of memory.
    if (new_pointer == NULL) {
        exit(1);
    }

    return new_pointer;
}

static void free_object(Obj* object) {
    switch (object->type) {
        case OBJ_STRING:
            ObjString* string = (ObjString*)object;
            FREE_ARRAY(char, string->chars, string->length + 1);
            FREE(ObjString, object);
            break;
    }
}

void free_objects() {
    Obj* object = vm.objects;
    while (object != NULL) {
        Obj* next = object->next;
        free_object(object);
        object = next;
    }
}
