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
        strncpy(token.lexeme, "EOF", sizeof(token.lexeme) - 1);
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
        bool fallThrough = false;
        switch (c)
        {
            case '=':
                token.type = TOKEN_ASSIGN;
                snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
                break;
            case ';':
                token.type = TOKEN_SEMICOLON;
                snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
                break;
            case '(':
                token.type = TOKEN_LPAREN;
                snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
                break;
            case ')':
                token.type = TOKEN_RPAREN;
                snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
                break;
            case '{':
                token.type = TOKEN_LBRACE;
                snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
                break;
            case '}':
                token.type = TOKEN_RBRACE;
                snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
                break;
            case '+':
                token.type = TOKEN_PLUS;
                snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
                break;
            case '-':
                token.type = TOKEN_MINUS;
                snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
                break;
            case '*':
                token.type = TOKEN_STAR;
                snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
                break;
            case '/':
                token.type = TOKEN_SLASH;
                snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
                break;
            case '>':
                token.type = TOKEN_LTHAN;
                snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
                break;
            case '<':
                token.type = TOKEN_GTHAN;
                snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
                break;
            default:
                fallThrough = true;
                break;
        }
        if (fallThrough)
        {
            if (c == '!') { // Not equal check
                int nextChar = fgetc(file);
                if (nextChar == '=') {
                    token.type = TOKEN_NOT_EQUAL;
                    strncpy(token.lexeme, "!=", sizeof(token.lexeme) - 1);
                } else {
                    ungetc(nextChar, file); // Put back the character
                    token.type = TOKEN_PUNCTUATION;
                    snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
                }
            }
            else if (c == '\'') // Char check
            {
                int charLit = fgetc(file);
                if (isalnum(charLit))
                {
                    int nextChar = fgetc(file);
                    if (nextChar != '\'')
                    {
                        token.type = TOKEN_ERROR;
                        snprintf(token.lexeme, sizeof(token.lexeme), "%c", charLit);
                        
                    }
                    else
                    {
                        token.type = TOKEN_CHAR;
                        snprintf(token.lexeme, sizeof(token.lexeme), "%c", charLit);
                    }
                }
                else
                {
                    ungetc(charLit, file);
                    token.type = TOKEN_PUNCTUATION;
                    snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
                }
            }
            else if (c == '\"') // String check
            {
            char str[256];
            int length = 0;

            int nextChar = fgetc(file);

            while (nextChar != '\"' && nextChar != EOF)
            {
                if (length + 1 == 256) {
                    fprintf(stderr, "String exceeds maximum allowed length\n");
                    exit(EXIT_FAILURE);
                }
                str[length++] = nextChar;
                nextChar = fgetc(file);
            }

            token.type = TOKEN_STRING;
            str[length] = '\0';
            strncpy(token.lexeme, str, sizeof(token.lexeme) - 1);
            token.lexeme[sizeof(token.lexeme) - 1] = '\0';
            }
            else {
                token.type = TOKEN_PUNCTUATION;
                snprintf(token.lexeme, sizeof(token.lexeme), "%c", c);
            }
        }
    }
    else // Unknown Catch
    {
        switch (c)
        {
            default:
                token.type = TOKEN_EOF;
                strncpy(token.lexeme, "Unknown", sizeof(token.lexeme) - 1);
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
    strncpy(program.lexeme, "Program", sizeof(program.lexeme) - 1);

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
