// Built-in
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Local
#include "include/jaclx.h"

// Test functions
void testLexer(FILE* file);

int main(int argc, char* argv[])
{   
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <fsourceFile>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* filePath = argv[1];
    FILE* file = fopen(filePath, "r");

    if (!file) 
    {
        fprintf(stderr, "Error opening file: %s\n", filePath);
        return EXIT_FAILURE;
    }

    testLexer(file);

    fclose(file);    
    return EXIT_SUCCESS;
}

void testLexer(FILE* file)
{
    initializeLexer(file);

    Token token;
    do 
    {
        token = getNextToken();
        printf("Token: Type=%d, lexeme='%s\n", token.type, token.lexeme);
    } while (token.type != TOKEN_EOF);

    cleanupLexer();
    fclose(file);

}