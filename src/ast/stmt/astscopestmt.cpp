/**
 * @file astscopestmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/stmt/astscopestmt.h>

ASTScopeStmt::ASTScopeStmt(ASTStmtSeqNode *body)
    : body(body)
{
}

ASTScopeStmt::~ASTScopeStmt() {
    delete body;
}

ASTStmtSeqNode *ASTScopeStmt::getBody() {
    return body;
}

void ASTScopeStmt::print(std::ostream & ss) {
    ss << "scope(";
    ss << std::endl;
    body->print(ss);
    ss << std::endl;
    ss << ")";
}
