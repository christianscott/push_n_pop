#include <stdio.h>
#include "lex.h"
#include "vendor/nothings/stb/stretchy_buffer.h"
#include "parse.h"

void print_commands(const Command *commands) {
    for (size_t i = 0; i < sb_count(commands); i++) {
        Command command = commands[i];
        switch (command.kind) {
            case COMMAND_KIND_PUSH:
                printf("Push(%d)\n", command.value);
                break;
            case COMMAND_KIND_POP:
                printf("Pop()\n");
                break;
        }
    }
}

int main(void) {
    LexResult result = lex("push 1\n"
                           "pop\n"
                           "push");
    switch (result.kind) {
        case LEX_RESULT_KIND_OK:
            print_commands(parse(result.tokens));
            return 0;
        case LEX_RESULT_KIND_ERR:
            lex_err_print(result.err);
            return 1;
    }
}
