//
// Created by Christian Scott on 20/3/21.
//

#ifndef PUSH_N_POP_LEX_H
#define PUSH_N_POP_LEX_H

#include "stdbool.h"

typedef enum {
    TOK_PUSH,
    TOK_POP,
    TOK_INT,
} TokenKind;

typedef struct sSpan {
    size_t offset;
    size_t len;
    size_t col;
    size_t line;
    const char *source;
} Span;

typedef struct sToken {
    TokenKind kind;
    Span span;
    union {
        // TOK_INT
        int value;
    };
} Token;

typedef enum {
    LEX_ERR_KIND_UNRECOGNISED_IDENTIFIER
} LexErrKind;

typedef struct sLexErr {
    LexErrKind kind;
    Span span;
} LexErr;

typedef struct sLexer {
    const char *source;
    size_t offset;
    size_t line;
    size_t col;
    char curr_char;

    bool has_err;
    LexErr err;
} Lexer;

void lex_err_print(LexErr *err);

LexErr *lex(char *source, Token *tokens);

#endif //PUSH_N_POP_LEX_H
