#include <stdio.h>
#include <assert.h>
#include "lex.h"
#include "stretchy_buffer.h"

int main(void) {
    LexResult result = lex("push 1\npop\n");

    assert(result.kind == LEX_RESULT_KIND_OK);

    Token *tokens = result.tokens;
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
    return 0;
}
