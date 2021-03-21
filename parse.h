#include "lex.h"

#ifndef PUSH_N_POP_PARSE_H
#define PUSH_N_POP_PARSE_H

typedef enum {
    COMMAND_KIND_PUSH,
    COMMAND_KIND_POP,
} CommandKind;

typedef struct sCommand {
    CommandKind kind;
    union {
        // COMMAND_KIND_PUSH
        int value;
    };
} Command;

Command *parse(Token *tokens);

#endif //PUSH_N_POP_PARSE_H
