#include <stdio.h>
#include "lex.h"
#include "stretchy_buffer.h"

void print_tokens(const Token *tokens) {
    for (size_t i = 0; i < sb_count(tokens); i++) {
        Token token = tokens[i];
        switch (token.kind) {
            case TOK_INT:
                printf("Int(%d)\n", token.value);
                break;
            case TOK_POP:
                printf("Pop\n");
                break;
            case TOK_PUSH:
                printf("Push\n");
                break;
        }
    }
}

int main(void) {
    LexResult result = lex("push 1\n"
                           "pop\n"
                           "pop    pip     push\n"
                           "push 1");
    switch (result.kind) {
        case LEX_RESULT_KIND_OK:
            print_tokens(result.tokens);
            break;
        case LEX_RESULT_KIND_ERR:
            lex_err_print(result.err);
            break;
    }
    return 0;
}
