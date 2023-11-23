#include <ctype.h>
#include <string.h> 

#include "../include/jaclx.h"
#include <stdlib.h>

static void skipWhiteSpaceAndComments(FILE* file)
{
    int c;

    while ((c = fgetc(file)) != EOF)
    {
        if (isspace(c))
        {
            continue; // Skip whitespace
        }
        else if (c == '/')
        {
            if ((c = fgetc(file)) == '/') // Checking for '//' start to comment
            {
                while ((c = fgetc(file)) != EOF && c != '\n'); // Ignore comment
            }
            else if (c == '*') // Block comment
            {
                while ((c = fgetc(file)) != EOF)
                {
                    if (c == '*' && (c = fgetc(file)) == '/')
                    {
                        break;
                    }
                }
            }
            else // Not a comment
            {
                ungetc(c, file);
                break;
            }
        }
        else // Not a whitespace or comment
        {
          ungetc(c, file);
          break;  
        }
    }
}


Token getNextToken(FILE* file)
{
    skipWhiteSpaceAndComments(file);

    int c = fgetc(file);

    Token token;

    if (c == EOF) // End of file
    {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "EOF");
    }
    else if (isalpha(c) || c == '_') // Identifier
    {
        ungetc(c, file);
        fscanf(file, "%255[a-zA-Z0-9_]", token.lexeme);
        token.type = TOKEN_ID; 

    }
    else if (isdigit(c)) // Int || Float
    {
        ungetc(c, file);
        fscanf(file, "%255[0-9.]", token.lexeme);
        if (strchr(token.lexeme, '.'))
        {
            token.type = TOKEN_FLOAT;
        }
        else
        {
            token.type = TOKEN_INT;
        }
    }
    else if (ispunct(c)) {
        if (c == '!') {
            int nextChar = fgetc(file);
            if (nextChar == '=') {
                token.type = TOKEN_NOT_EQUAL;
                strcpy(token.lexeme, "!=");
            } else {
                ungetc(nextChar, file); // Put back the character
                token.type = TOKEN_PUNCTUATION;
                snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
            }
        } else {
            token.type = TOKEN_PUNCTUATION;
            snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
        }
    }
    else // Character
    {
        switch (c)
        {
            case '+':
                token.type = TOKEN_PLUS;
                break;
            case '-':
                token.type = TOKEN_MINUS;
                break;
            case '*':
                token.type = TOKEN_STAR;
                break;
            case '/':
                token.type = TOKEN_SLASH;
                break;
            case '=':
                token.type = TOKEN_ASSIGN;
                break;
            case ';':
                token.type = TOKEN_SEMICOLON;
                break;
            case '(':
                token.type = TOKEN_LPAREN;
                break;
            case ')':
                token.type = TOKEN_RPAREN;
                break;
            default:
                token.type = TOKEN_EOF;
                strcpy(token.lexeme, "Unknown");
        }
    }
    return token;
}

TokenListNode* addToken(TokenListNode* parent, TokenListNode* node)
{
    if (parent == NULL)
    {
        return NULL;
    }
    else if (node == NULL)
    {
        return parent;
    }

    TokenListNode* currentNode = parent;
    while (currentNode->next != NULL)
    {
        currentNode = currentNode->next;
    }

    currentNode->next = node;
    
    return node;
}

TokenListNode* createTokenListNode(Token token)
{
    TokenListNode* tokenNode = (TokenListNode*)malloc(sizeof(TokenListNode));
    if (tokenNode == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for TokenListNode\n");
        exit(EXIT_FAILURE);
    }

    tokenNode->token = token;
    tokenNode->next = NULL;

    return tokenNode;
}

void deleteTokenList(TokenListNode* root)
{
    if (root == NULL)
    {
        return;
    }

    deleteTokenList(root->next);
    root->next = NULL;

    free(root);
}

TokenListNode* tokenize(FILE* file)
{   
    Token program;
    program.type = TOKEN_PROGRAM;
    strcpy(program.lexeme, "Program");

    TokenListNode* root = createTokenListNode(program);

    Token token;
    TokenListNode* currentNode = root;
    do 
    {
        token = getNextToken(file);
        currentNode = addToken(currentNode, createTokenListNode(token));
        
    } while (token.type != TOKEN_EOF);

    return root;
}
