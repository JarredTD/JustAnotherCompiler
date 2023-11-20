// util.h
#ifndef UTIL_H
#define UTIL_H

typedef enum
{
    TOKEN_INT,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF
} TokenType;

typedef struct 
{
    TokenType type;
    int value; // For Integers
} Token;
#endif // UTIL.H