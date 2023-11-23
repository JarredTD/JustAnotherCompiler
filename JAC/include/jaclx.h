// JACLX_H
#ifndef JAXLX_H
#define JAXLX_H

#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Token Types
 * 
 */
typedef enum
{
    // Program
    TOKEN_PROGRAM,

    // Literals
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_CHAR,
    
    // Delimiters
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    
    // Operators
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_ASSIGN,
    TOKEN_LTHAN,
    TOKEN_GTHAN,
    TOKEN_NOT_EQUAL,

    // Punctuation
    TOKEN_PUNCTUATION,

    // End of file
    TOKEN_EOF,

    // AST Node Types
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_RETURN,
    
    // Unknown Token
    TOKEN_UNKNOWN,

    // Error
    TOKEN_ERROR
} TokenType;

/**
 * @brief Token Struct
 * 
 * @param TokenType type
 * @param char[] The lexeme
 */
typedef struct
{
    TokenType type;
    char lexeme[256];
} Token;

/**
 * @brief LinkedList of Tokens
 * 
 * @param Token node token
 * @param TokenListNode* next node
 */
typedef struct
{
    Token token;
    struct TokenListNode* next;
} TokenListNode;

/**
 * @brief Create a tokenListNode
 * 
 * @param Token token to made a node of
 * @return TokenListNode* 
 */
TokenListNode* createTokenListNode(Token token);

/**
 * @brief Consumes and skips whitespaces and comments
 * 
 */
static void skipWhiteSpaceAndComments();

/**
 * @brief Get the next Token object
 * 
 * @param file File to consume characters from
 * @return Token 
 */
Token getNextToken(FILE* file);

/**
 * @brief Adds a node to the end of the TokenList
 * 
 * @param root of the list
 * @param node to be added
 * 
 * @return TokenListNode*, the head, IE the just added node
 */
TokenListNode* addToken(TokenListNode* root, TokenListNode node);

void deleteTokenList(TokenListNode* root);

/**
 * @brief Consumes all characters from a source file and creates an
 * array of tokens
 * 
 * @return TokenListNode* Root
 */
TokenListNode* tokenize(FILE* file);


#endif // JAXLX_H