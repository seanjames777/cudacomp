/**
 * @file astscope.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astscope.h>

ASTScope::ASTScope(ASTStmtNode *body)
    : body(body)
{
}

ASTScope::~ASTScope() {
    delete body;
}

ASTStmtNode *ASTScope::getBody() {
    return body;
}
