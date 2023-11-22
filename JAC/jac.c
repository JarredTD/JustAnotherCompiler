// Built-in
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Local
#include "include/jaclx.h"
#include "include/jacp.h"

// Test functions
void testLexer(FILE* file);
void testParser(FILE* file);

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

    testParser(file);

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

void testParser(FILE* file)
{
    initializeLexer(file);

    AstNode* syntaxTree = parse();

    if (syntaxTree != NULL)
    {
        printf("\nParser: Syntax Tree Structure:\n");
        displayAST(syntaxTree, 0);

        deleteAST(syntaxTree);
    }
    else
    {
        fprintf(stderr, "Parser: Parsing failed\n");
    }

    cleanupLexer();
}