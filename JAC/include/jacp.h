// JACP_H
#ifndef JACP_H
#define JACP_H
#include "jaclx.h"

/**
 * @brief Node of an Abstract Syntax Tree
 * 
 * @param TokenType: Type
 * @param Char[] Lexeme
 * @param AstNode** Children
 * @param Int Number of children
 */
typedef struct AstNode
{
    TokenType type;
    char lexeme[256];
    struct AstNode** children;
    int numChildren;
} AstNode;

/**
 * @brief Create an AstNode
 * 
 * @param type The type of node to create
 * @param lexeme The lexeme of the node
 * @return AstNode* 
 */
AstNode* createAstNode(TokenType type, const char* lexeme);

/**
 * @brief Adds a child node to a parent node. 
 * 
 * @param parent The parent node to be added to
 * @param child The child node to be added
 */
void addChild(AstNode* parent, AstNode* child);

/**
 * @brief Deletes the entire Abstract Syntax Tree by recursively 
 * deleting all children of the root node
 * 
 * @param root The node to recursively (and inclusively) free.
 */
void deleteAst(AstNode* root);


#endif // JACP_H