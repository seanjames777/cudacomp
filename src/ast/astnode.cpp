/**
 * @file astnode.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astnode.h>

ASTMalformedException::ASTMalformedException()
    : runtime_error("Malformed AST")
{
}

ASTNode::~ASTNode() {
}
