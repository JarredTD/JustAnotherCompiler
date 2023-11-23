// Built-in
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Local
#include "include/jaclx.h"

// Test functions
void tester(FILE* file);

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

    tester(file);

    fclose(file);    
    return EXIT_SUCCESS;
}

void tester(FILE* file)
{
    TokenListNode* tokenList = tokenize(file);

    if (tokenList == NULL)
    {
        return;
    }

    TokenListNode* currentNode = tokenList;
    while(currentNode->next != NULL)
    {
        Token token = currentNode->token;
        printf("Token: Type=%d, lexeme='%s'\n", token.type, token.lexeme);
        currentNode = currentNode->next;
    }

    deleteTokenList(tokenList);
}