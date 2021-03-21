#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "parse.h"
#include "stretchy_buffer.h"

bool is_ascii_alpha(const char c) {
    return (ASCII_UPPER_A <= c && c <= ASCII_UPPER_Z)
           || (ASCII_LOWER_A <= c && c <= ASCII_LOWER_Z);
}

bool is_ascii_numeric(const char c) {
    return ASCII_ZERO <= c && c <= ASCII_NINE;
}

bool is_ascii_alpha_numeric(const char c) {
    return is_ascii_alpha(c) || is_ascii_numeric(c);
}

bool is_whitespace(const char c) {
    return c == '\n' || c == ' ' || c == '\t' || c == '\r';
}

Lexer *lexer_new(const char *source) {
    Lexer *lexer = malloc(sizeof(Lexer));
    lexer->current_char_idx = 0;
    lexer->current_char = source[0];
    lexer->source = source;

    Token *tokens = NULL;

    lexer->tokens = tokens;

    return lexer;
}

void lexer_next(Lexer *lexer) {
    lexer->current_char = lexer->source[++lexer->current_char_idx];
}

void lexer_lex_identifier(Lexer *lexer) {
    size_t start = lexer->current_char_idx;
    while (is_ascii_alpha_numeric(lexer->current_char)) {
        lexer_next(lexer);
    }

    size_t len = lexer->current_char_idx - start;
    char *identifier = malloc(sizeof(char) * len + 1);
    memcpy(identifier, lexer->source + start, len);
    identifier[len] = '\0';

    if (strcmp(identifier, "push") == 0) {
        Token token = { .kind = TOK_PUSH };
        sb_push(lexer->tokens, token);
    } else if (strcmp(identifier, "pop") == 0) {
        Token token = { .kind = TOK_POP };
        sb_push(lexer->tokens, token);
    } else {
        fprintf(stderr, "did not recognise token of kind %s", identifier);
        exit(1);
    }
}

void lexer_lex_int(Lexer *lexer) {
    size_t start = lexer->current_char_idx;
    while (is_ascii_numeric(lexer->current_char)) {
        lexer_next(lexer);
    }

    size_t len = lexer->current_char_idx - start;
    char *num = malloc(sizeof(char) * len + 1);
    memcpy(num, lexer->source + start, len);
    num[len] = '\0';

    Token token = { .kind = TOK_INT, .value = atoi(num) };
    sb_push(lexer->tokens, token);
}

LexResult lex(char *source) {
    Lexer *lexer = lexer_new(source);
    while (lexer->current_char != '\0') {
        if (is_whitespace(lexer->current_char)) {
            lexer_next(lexer);
            continue;
        }

        if (is_ascii_alpha(lexer->current_char)) {
            lexer_lex_identifier(lexer);
            continue;
        }

        if (is_ascii_numeric(lexer->current_char)) {
            lexer_lex_int(lexer);
        }
    }

    LexResult result = {
            .kind = LEX_RESULT_KIND_OK,
            .tokens = lexer->tokens
    };

//    free(lexer);
    return result;
}

