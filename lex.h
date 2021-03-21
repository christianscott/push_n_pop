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
    union {
        // LEX_ERR_KIND_UNRECOGNISED_IDENTIFIER
        Span span;
    };
} LexErr;

typedef struct sLexer {
    const char *source;
    size_t offset;
    size_t line;
    size_t col;
    char curr_char;

    Token *tokens;

    bool has_err;
    LexErr err;
} Lexer;

typedef enum {
    LEX_RESULT_KIND_OK,
    LEX_RESULT_KIND_ERR
} LexResultKind;

typedef struct sLexResult {
    LexResultKind kind;
    union {
        // LEX_RESULT_KIND_OK
        Token *tokens;
        // LEX_RESULT_KIND_ERR
        LexErr err;
    };
} LexResult;

void lex_err_print(LexErr err);

LexResult lex(char *source);

#endif //PUSH_N_POP_LEX_H
