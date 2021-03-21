//
// Created by Christian Scott on 21/3/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "parse.h"
#include "vendor/nothings/stb/stretchy_buffer.h"

Command *parse(Token *tokens) {
    Command *commands = NULL;
    size_t i = 0;
    while (i < sb_count(tokens)) {
        Token token = tokens[i++];
        switch (token.kind) {
            case TOK_PUSH: {
                assert(i < sb_count(tokens));
                Token next_token = tokens[i++];
                assert(next_token.kind == TOK_INT);
                Command command = {.kind = COMMAND_KIND_PUSH, .value = next_token.value};
                sb_push(commands, command);
                break;
            }
            case TOK_POP: {
                Command command = {.kind = COMMAND_KIND_POP};
                sb_push(commands, command);
                break;
            }
            case TOK_INT: {
                fprintf(stderr, "unexpected int %d", token.value);
                exit(1);
            }
        }
    }
    return commands;
}
