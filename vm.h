//
// Created by Christian Scott on 20/3/21.
//

#ifndef PUSH_N_POP_VM_H
#define PUSH_N_POP_VM_H

typedef enum {
    OBJ_INT,
    OBJ_PAIR
} ObjectKind;

typedef struct sObject {
    unsigned char marked;
    ObjectKind kind;
    struct sObject *next;

    union {
        // OBJ_INT
        int value;

        // OBJ_PAIR
        struct {
            struct sObject *head;
            struct sObject *tail;
        };
    };
} Object;

#define PUSH_N_POP_VM_H_STACK_MAX 256

typedef struct {
    int n_objects;
    int max_objects;
    Object *first_object;
    Object *stack[PUSH_N_POP_VM_H_STACK_MAX];
    int stack_size;
} VM;

VM *vm_new();

Object *object_new(ObjectKind kind);

void object_mark(Object *object);

void vm_mark_all(VM *vm);

void vm_sweep(VM *vm);

void vm_collect_garbage(VM *vm);

Object *vm_create_object(VM *vm, ObjectKind kind);

void vm_push(VM *vm, Object *value);

Object *vm_pop(VM *vm);

void vm_push_int(VM *vm, int value);

Object *vm_push_pair(VM *vm);


#endif //PUSH_N_POP_VM_H
