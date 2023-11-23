#include "../include/jacp.h"
#include <stdlib.h>

AstNode* createAstNode(TokenType type, const char* lexeme)
{
    AstNode* node = (AstNode*)malloc(sizeof(AstNode));
    node->type = type;
    strcpy(node->lexeme, lexeme);
    node->children = NULL;
    node->numChildren = 0;

    return node;
}

void addChild(AstNode* parent, AstNode* child)
{
    parent->numChildren++;
    parent->children = (AstNode**)realloc(parent->children, sizeof(AstNode) * parent->numChildren);
    parent->children[parent->numChildren-1] = child;
}

void deleteAst(AstNode* root)
{
    if (root == NULL)
    {
        return;
    }

    for (int i = 0; i < root->numChildren; i++)
    {
        deleteAst(root->children[i]);
    }

    free(root->children);
    free(root);
}
