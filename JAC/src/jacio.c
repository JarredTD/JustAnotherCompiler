#include <stdlib.h>
#include <ctype.h>
#include "../include/jacio.h"

Token getToken(FILE* file) {
    int c = fgetc(file);

    while (isspace(c)) 
    {
        c = fgetc(file);
    }

    Token token;

    switch (c)
    {
        case '+':
            token.type = TOKEN_PLUS;
            break;
        case '-':
            token.type = TOKEN_MINUS;
            break;
        case '*':
            token.type = TOKEN_MULTIPLY;
            break;
        case '/':
            token.type = TOKEN_DIVIDE;
            break;
        case '(':
            token.type = TOKEN_LPAREN;
            break;
        case ')':
            token.type = TOKEN_RPAREN;
            break;
        case EOF:
            token.type = TOKEN_EOF;
            break;
        default:
        if (isdigit(c)) 
        {
            ungetc(c, file);
            fscanf_s(file, "%d", &token.value);
            token.type = TOKEN_INT;
        }
        else 
        {
            fprintf(stderr, "Unknown character: %c\n", c);
            exit(EXIT_FAILURE);
        }
    }

    return token;
}