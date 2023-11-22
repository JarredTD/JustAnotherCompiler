// JACP_H
#ifndef JACP_H
#define JACP_H
#include "jaclx.h"

typedef struct AstNode
{
    TokenType type;
    char lexeme[256];
    struct AstNode* children;
    struct AstNode* next;
} AstNode;

AstNode* createAstNode(TokenType type, const char* lexeme);
void deleteAstNode(AstNode* node);
AstNode* parseInt();
AstNode* parseFactor();
AstNode* parseTerm();
AstNode* parseExpression();
AstNode* parseAssignment();
AstNode* parseStatement();
AstNode* parseBlock();
AstNode* parse();
void displayAST(AstNode* node, int depth);
void deleteAST(AstNode* root);

#endif // JACP_H