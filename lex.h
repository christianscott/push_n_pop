//
// Created by Christian Scott on 20/3/21.
//

#ifndef PUSH_N_POP_LEX_H
#define PUSH_N_POP_LEX_H

#define ASCII_ZERO 48
#define ASCII_NINE 57
#define ASCII_UPPER_A 65
#define ASCII_UPPER_Z 90
#define ASCII_LOWER_A 97
#define ASCII_LOWER_Z 122

typedef enum {
    TOK_PUSH,
    TOK_POP,
    TOK_INT,
} TokenKind;

typedef struct sToken {
    TokenKind kind;
    union {
        // TOK_INT
        int value;
    };
} Token;

typedef enum {
    INSTR_PUSH,
    INSTR_PAIR,
    INSTR_POP
} InstructionKind;

typedef struct sInstruction {
    InstructionKind kind;
    union {
        // INSTR_PUSH
        int value;
    };
} Instruction;

typedef struct sLexer {
    Token *tokens;

    const char *source;
    size_t current_char_idx;
    char current_char;
} Lexer;

typedef enum {
    LEX_RESULT_KIND_OK,
} LexResultKind;

typedef struct sLexResult {
    LexResultKind kind;
    union {
        // LEX_RESULT_KIND_OK
        Token *tokens;
    };
} LexResult;

LexResult lex(char *source);

#endif //PUSH_N_POP_LEX_H
