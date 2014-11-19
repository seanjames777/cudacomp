/**
 * @file astscope.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/stmt/astscope.h>

ASTScope::ASTScope(ASTStmtSeqNode *body)
    : body(body)
{
}

ASTScope::~ASTScope() {
    delete body;
}

ASTStmtSeqNode *ASTScope::getBody() {
    return body;
}

void ASTScope::print(std::ostream & ss) {
    ss << "scope(";
    ss << std::endl;
    body->print(ss);
    ss << std::endl;
    ss << ")";
}
