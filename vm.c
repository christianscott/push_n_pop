//
// Created by Christian Scott on 20/3/21.
//

#include <assert.h>
#include <stdlib.h>
#include "vm.h"

#define PUSH_N_POP_VM_C_INITIAL_VM_THRESHOLD 256

VM *vm_new() {
    VM *vm = malloc(sizeof(VM));
    vm->stack_size = 0;
    vm->n_objects = 0;
    vm->max_objects = PUSH_N_POP_VM_C_INITIAL_VM_THRESHOLD;
    return vm;
}

Object *object_new(ObjectKind kind) {
    Object *object = malloc(sizeof(Object));
    object->kind = kind;
    object->marked = 0;
    return object;
}

void object_mark(Object *object) {
    if (object->marked) {
        return;
    }

    object->marked = 1;

    if (object->kind == OBJ_PAIR) {
        object_mark(object->head);
        object_mark(object->tail);
    }
}

void vm_mark_all(VM *vm) {
    for (int i = 0; i < vm->stack_size; i++) {
        object_mark(vm->stack[i]);
    }
}

void vm_sweep(VM *vm) {
    Object **object = &vm->first_object;
    while (*object) {
        if (!(*object)->marked) {
            Object *unreached = *object;
            *object = unreached->next;
            free(unreached);
            vm->n_objects--;
        } else {
            (*object)->marked = 0;
            object = &(*object)->next;
        }
    }
}

void vm_collect_garbage(VM *vm) {
    vm_mark_all(vm);
    vm_sweep(vm);

    vm->max_objects = vm->n_objects * 2;
}

Object *vm_create_object(VM *vm, ObjectKind kind) {
    if (vm->n_objects == vm->max_objects) {
        vm_collect_garbage(vm);
    }

    Object *object = object_new(kind);
    object->next = vm->first_object;

    vm->first_object = object;
    vm->n_objects++;

    return object;
}

void vm_push(VM *vm, Object *value) {
    assert(("Stack overflow", vm->stack_size < PUSH_N_POP_VM_H_STACK_MAX));
    vm->stack[vm->stack_size++] = value;
}

Object *vm_pop(VM *vm) {
    assert(("Stack underflow", vm->stack_size > 0));
    return vm->stack[--vm->stack_size];
}

void vm_push_int(VM *vm, int value) {
    Object *object = vm_create_object(vm, OBJ_INT);
    object->value = value;
    vm_push(vm, object);
}

Object *vm_push_pair(VM *vm) {
    Object *object = vm_create_object(vm, OBJ_PAIR);
    object->tail = vm_pop(vm);
    object->head = vm_pop(vm);
    vm_push(vm, object);
    return object;
}
