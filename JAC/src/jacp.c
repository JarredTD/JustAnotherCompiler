#include "../include/jacp.h"
#include <stdlib.h>
#include <string.h>

FILE* sourceFile;

AstNode* createAstNode(TokenType type, const char* lexeme)
{
    AstNode* node = (AstNode*)malloc(sizeof(AstNode));
    if (node != NULL)
    {
        node->type = type;
        strcpy(node->lexeme, lexeme);
        node->children = NULL;
        node->next = NULL;
    }


    return node;
}

void deleteAstNode(AstNode* node)
{
    if (node != NULL)
    {
        deleteAstNode(node->children);
        deleteAstNode(node->next);
        free(node);
    }
}

AstNode* parseInt()
{
    Token token = getNextToken();
    if (token.type == TOKEN_INT)
    {
        return createAstNode(TOKEN_INT, token.lexeme);
    }
    else
    {
        fprintf(stderr, "Error: Expected an integer, got %s\n", token.lexeme);
        return NULL;
    }
}

AstNode* parseFactor()
{
    Token token = getNextToken();
    if (token.type == TOKEN_INT)
    {
        return createAstNode(TOKEN_INT, token.lexeme);
    }
    else if (token.type == TOKEN_LPAREN)
    {
        AstNode* expression = parseExpression();
        if (expression != NULL && getNextToken().type == TOKEN_RPAREN)
        {
            return expression;
        }
        else 
        {
            fprintf(stderr, "Error: Expected ')'\n");
            return NULL;
        }
    }
    else
    {
        fprintf(stderr, "Error: Unexpected token %s\n", token.lexeme);
        return NULL;
    }
}

AstNode* parseTerm()
{
    AstNode* left = parseFactor();

    while(1) 
    {
        Token token = getNextToken();
        if (token.type == TOKEN_STAR || token.type == TOKEN_SLASH)
        {
            AstNode* right = parseFactor();

            if (right != NULL)
            {
                AstNode* operatorNode = createAstNode(token.type, token.lexeme);
                operatorNode->children = right;
                operatorNode->next = NULL;

                AstNode* current = left;
                while (current->next != NULL)
                {
                    current = current->next;
                }
                current->next = operatorNode;   
            }
            else
            {
                fprintf(stderr, "Error: Expected a factor after %s\n", token.lexeme);
                deleteAstNode(left);
                return NULL;
            }
        }
        else
        {
            ungetc(token.lexeme[0], sourceFile);
            break;
        }
    }
    
    return left;
}

AstNode* parseExpression()
{
    AstNode* left = parseFactor();

    while(1) 
    {
        Token token = getNextToken();
        if (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS)
        {
            AstNode* right = parseFactor();

            if (right != NULL)
            {
                AstNode* operatorNode = createAstNode(token.type, token.lexeme);
                operatorNode->children = right;
                operatorNode->next = NULL;

                AstNode* current = left;
                while (current->next != NULL)
                {
                    current = current->next;
                }
                current->next = operatorNode;   
            }
            else
            {
                fprintf(stderr, "Error: Expected a term after %s\n", token.lexeme);
                deleteAstNode(left);
                return NULL;
            }
        }
        else
        {
            ungetc(token.lexeme[0], sourceFile);
            break;
        }
    }
    
    return left;
}

AstNode* parseAssignment() {
    Token identifierToken = getNextToken();
    if (identifierToken.type != TOKEN_ID) {
        fprintf(stderr, "Error: Expected an identifier in assignment\n");
        return NULL;
    }

    Token equalToken = getNextToken();
    if (equalToken.type != TOKEN_ASSIGN) {
        fprintf(stderr, "Error: Expected '=' in assignment\n");
        return NULL;
    }

    AstNode* expression = parseExpression();
    if (expression == NULL) {
        fprintf(stderr, "Error: Invalid expression in assignment\n");
        return NULL;
    }

    AstNode* assignmentNode = createAstNode(TOKEN_ASSIGN, "=");
    assignmentNode->children = expression;

    AstNode* identifierNode = createAstNode(TOKEN_ID, identifierToken.lexeme);
    identifierNode->next = assignmentNode;

    return identifierNode;
}

AstNode* parseStatement() {
    return parseAssignment();
}

AstNode* parseBlock() {
    Token token = getNextToken();
    if (token.type != TOKEN_LBRACE) {
        fprintf(stderr, "Error: Expected '{' to start a block\n");
        return NULL;
    }

    AstNode* blockNode = createAstNode(TOKEN_LBRACE, "{");
    AstNode* currentStatement = blockNode;

    while (1) {
        AstNode* statement = parseStatement();

        if (statement == NULL) {
            fprintf(stderr, "Error: Invalid statement in block\n");
            deleteAstNode(blockNode);
            return NULL;
        }

        currentStatement->next = statement;
        currentStatement = statement;

        token = getNextToken();
        if (token.type == TOKEN_RBRACE) {
            break;
        } else if (token.type != TOKEN_SEMICOLON) {
            fprintf(stderr, "Error: Expected ';' after statement\n");
            deleteAstNode(blockNode);
            return NULL;
        }
    }

    return blockNode;
}

AstNode* parse()
{
    return parseExpression();
}

void displayAST(AstNode* node, int depth) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("  "); 
    }
    printf("Type: %d, Lexeme: '%s'\n", node->type, node->lexeme);

    displayAST(node->children, depth + 1);

    displayAST(node->next, depth);
}   

void deleteAST(AstNode* root) {
    if (root == NULL) {
        return;
    }

    deleteAST(root->children);
    deleteAST(root->next);

    deleteAstNode(root);
}