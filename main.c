#include <stdio.h>
#include "lex.h"
#include "vendor/nothings/stb/stretchy_buffer.h"
#include "parse.h"
#include "vm.h"

void vm_run_commands(VM *vm, Command *commands) {
    for (size_t i = 0; i < sb_count(commands); i++) {
        Command command = commands[i];
        switch (command.kind) {
            case COMMAND_KIND_PUSH:
                vm_push_int(vm, command.value);
                printf("pushed %d\n", command.value);
                break;
            case COMMAND_KIND_POP: {
                Object *o = vm_pop(vm);
                printf("popped %d\n", o->value);
                break;
            }
        }
    }
}

int main(void) {
    Token tokens[] = {};
    char *source = "push 1\n"
                   "pop\n"
                   "push 2"
                   "pop\n";
    LexErr *err = lex(source, tokens);
    if (err != NULL) {
        lex_err_print(err);
        return 1;
    }

    Command *commands = NULL;
    commands = parse(tokens);

    VM *vm = vm_new();
    vm_run_commands(vm, commands);

    return 0;
}
