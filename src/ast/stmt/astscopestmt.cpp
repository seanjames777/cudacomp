/**
 * @file astscopestmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/stmt/astscopestmt.h>

ASTScopeStmt::ASTScopeStmt(std::shared_ptr<ASTStmtSeqNode> body)
    : body(body)
{
}

ASTScopeStmt::~ASTScopeStmt() {
}

std::shared_ptr<ASTStmtSeqNode> ASTScopeStmt::getBody() {
    return body;
}

void ASTScopeStmt::print(std::ostream & ss) {
}
