#include <ctype.h>
#include <string.h> 

#include "../include/jaclx.h"

static FILE* sourceFile;
static Token currentToken;

void initializeLexer(FILE* file)
{
    sourceFile = file;
    skipWhiteSpaceAndComments(); // Ignore initial whitespace or comments
}

static void skipWhiteSpaceAndComments()
{
    int c;

    while ((c = fgetc(sourceFile)) != EOF)
    {
        if (isspace(c))
        {
            continue; // Skip whitespace
        }
        else if (c == '/')
        {
            if ((c = fgetc(sourceFile)) == '/') // Checking for '//' start to comment
            {
                while ((c = fgetc(sourceFile)) != EOF && c != '\n'); // Ignore comment
            }
            else if (c == '*') // Block comment
            {
                while ((c = fgetc(sourceFile)) != EOF)
                {
                    if (c == '*' && (c = fgetc(sourceFile)) == '/')
                    {
                        break;
                    }
                }
            }
            else // Not a comment
            {
                ungetc(c, sourceFile);
                break;
            }
        }
        else // Not a whitespace or comment
        {
          ungetc(c, sourceFile);
          break;  
        }
    }
}

Token getNextToken()
{
    skipWhiteSpaceAndComments();
    int c = fgetc(sourceFile);

    if (c == EOF) // End of file
    {
        currentToken.type = TOKEN_EOF;
        strcpy(currentToken.lexeme, "EOF");
    }
    else if (isalpha(c) || c == '_') // Identifier
    {
        ungetc(c, sourceFile);
        fscanf(sourceFile, "%255[a-zA-Z0-9_]", currentToken.lexeme);
        currentToken.type = TOKEN_ID;
    }
    else if (isdigit(c)) // Int || Float
    {
        ungetc(c, sourceFile);
        fscanf(sourceFile, "%255[0-9.]", currentToken.lexeme);
        if (strchr(currentToken.lexeme, '.'))
        {
            currentToken.type = TOKEN_FLOAT;
        }
        else
        {
            currentToken.type = TOKEN_INT;
        }
    }
    else if (ispunct(c)) {
    if (c == '!') {
        int nextChar = fgetc(sourceFile);
        if (nextChar == '=') {
            currentToken.type = TOKEN_NOT_EQUAL;
            strcpy(currentToken.lexeme, "!=");
        } else {
            ungetc(nextChar, sourceFile); // Put back the character
            currentToken.type = TOKEN_PUNCTUATION;
            snprintf(currentToken.lexeme, sizeof(currentToken.lexeme), "%c", c);
        }
    } else {
        currentToken.type = TOKEN_PUNCTUATION;
        snprintf(currentToken.lexeme, sizeof(currentToken.lexeme), "%c", c);
    }
}

    else // Character
    {
        switch (c)
        {
            case '+':
                currentToken.type = TOKEN_PLUS;
                break;
            case '-':
                currentToken.type = TOKEN_MINUS;
                break;
            case '*':
                currentToken.type = TOKEN_STAR;
                break;
            case '/':
                currentToken.type = TOKEN_SLASH;
                break;
            case '=':
                currentToken.type = TOKEN_ASSIGN;
                break;
            case ';':
                currentToken.type = TOKEN_SEMICOLON;
                break;
            case '(':
                currentToken.type = TOKEN_LPAREN;
                break;
            case ')':
                currentToken.type = TOKEN_RPAREN;
                break;
            default:
                currentToken.type = TOKEN_EOF;
                strcpy(currentToken.lexeme, "Unknown");
        }
    }
    return currentToken;
}

void cleanupLexer()
{
    fclose(sourceFile);
}
