// JACLX_H
#ifndef JAXLX_H
#define JAXLX_H

#include <stdio.h>

typedef enum
{
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_ASSIGN,
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LTHAN,
    TOKEN_GTHAN,
    TOKEN_NOT_EQUAL,
    TOKEN_PUNCTUATION,
    TOKEN_EOF
} TokenType;

typedef struct
{
    TokenType type;
    char lexeme[256];
} Token;

void initializeLexer(FILE* file);
void cleanupLexer();
static void skipWhiteSpaceAndComments();
Token getNextToken();

#endif // JAXLX_H