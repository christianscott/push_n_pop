#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lex.h"
#include "vendor/nothings/stb/stretchy_buffer.h"

#define ASCII_ZERO 48
#define ASCII_NINE 57
#define ASCII_UPPER_A 65
#define ASCII_UPPER_Z 90
#define ASCII_LOWER_A 97
#define ASCII_LOWER_Z 122

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

    lexer->offset = 0;
    lexer->line = 0;
    lexer->col = 0;

    lexer->source = source;
    lexer->curr_char = source[0];

    Token *tokens = NULL;
    lexer->tokens = tokens;

    return lexer;
}

void lexer_free_except_tokens(Lexer *lexer) {
    free(lexer);
}

void lexer_next(Lexer *lexer) {
    if (lexer->curr_char == '\n') {
        lexer->line++;
        lexer->col = 0;
    } else {
        lexer->col++;
    }

    lexer->curr_char = lexer->source[++lexer->offset];
}

Span lexer_create_span(Lexer *lexer, size_t len) {
    Span span = {
            .source = lexer->source,
            .offset = lexer->offset - len,
            .line = lexer->line,
            .col = lexer->col - len,
            .len = len
    };
    return span;
}

void lexer_lex_identifier(Lexer *lexer) {
    size_t start = lexer->offset;
    while (is_ascii_alpha_numeric(lexer->curr_char)) {
        lexer_next(lexer);
    }

    size_t len = lexer->offset - start;
    char *identifier = malloc(sizeof(char) * len + 1);
    memcpy(identifier, lexer->source + start, len);
    identifier[len] = '\0';

    Span span = lexer_create_span(lexer, len);
    if (strcmp(identifier, "push") == 0) {
        Token token = {.kind = TOK_PUSH, .span = span};
        sb_push(lexer->tokens, token);
    } else if (strcmp(identifier, "pop") == 0) {
        Token token = {.kind = TOK_POP, .span = span};
        sb_push(lexer->tokens, token);
    } else {
        LexErr err = {
                .kind = LEX_ERR_KIND_UNRECOGNISED_IDENTIFIER,
                .span = span,
        };
        lexer->has_err = true;
        lexer->err = err;
    }
}

void lexer_lex_int(Lexer *lexer) {
    size_t start = lexer->offset;
    while (is_ascii_numeric(lexer->curr_char)) {
        lexer_next(lexer);
    }

    size_t len = lexer->offset - start;
    char *num = malloc(sizeof(char) * len + 1);
    memcpy(num, lexer->source + start, len);
    num[len] = '\0';

    Span span = lexer_create_span(lexer, len);
    Token token = {.kind = TOK_INT, .span = span, .value = atoi(num)};

    sb_push(lexer->tokens, token);
}

const char *span_get_text(Span span) {
    char *text = malloc(sizeof(char) * (span.len + 1));
    memcpy(text, span.source + span.offset, span.len);
    text[span.len] = '\0';
    return text;
}

const char *span_get_full_line(Span span) {
    size_t line_start = span.offset;
    while (line_start > 0 && span.source[line_start-1] != '\n') {
        line_start -= 1;
    }

    size_t line_len = span.len;
    while (span.source[line_start + line_len] != '\0' && span.source[line_start + line_len] != '\n') {
        line_len += 1;
    }

    char *line = malloc(sizeof(char) * line_len);
    memcpy(line, span.source + line_start, line_len);
    line[line_len] = '\0';

    return line;
}

char *repeat_char(char c, int times) {
    char *repeated = malloc(sizeof(char) * (times + 1));
    for (int i = 0; i < times; i++) {
        repeated[i] = c;
    }
    repeated[times] = '\0';
    return repeated;
}

void lex_err_print(LexErr err) {
    switch (err.kind) {
        case LEX_ERR_KIND_UNRECOGNISED_IDENTIFIER: {
            printf("[eval]:%zu:%zu LexErr\n", err.span.line, err.span.col);
            printf("%s\n", span_get_full_line(err.span));
            printf("%s^ unrecognised identifier \"%s\"\n", repeat_char(' ', err.span.col), span_get_text(err.span));
        }
    }
}

LexResult lexer_into_lex_result(Lexer *lexer) {
    LexResult result;
    if (lexer->has_err) {
        result.kind = LEX_RESULT_KIND_ERR;
        result.err = lexer->err;
    } else {
        result.kind = LEX_RESULT_KIND_OK;
        result.tokens = lexer->tokens;
    }
    lexer_free_except_tokens(lexer);
    return result;
}

LexResult lex(char *source) {
    Lexer *lexer = lexer_new(source);
    while (lexer->curr_char != '\0') {
        if (lexer->has_err) {
            return lexer_into_lex_result(lexer);
        }

        if (is_whitespace(lexer->curr_char)) {
            lexer_next(lexer);
            continue;
        }

        if (is_ascii_alpha(lexer->curr_char)) {
            lexer_lex_identifier(lexer);
            continue;
        }

        if (is_ascii_numeric(lexer->curr_char)) {
            lexer_lex_int(lexer);
        }
    }

    return lexer_into_lex_result(lexer);
}

