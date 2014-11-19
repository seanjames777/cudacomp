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

void ASTScope::print(std::ostream & ss) {
    ss << "scope(";
    ss << std::endl;
    body->print(ss);
    ss << std::endl;
    ss << ")";
}
