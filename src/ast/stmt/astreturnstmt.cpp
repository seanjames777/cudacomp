/**
 * @file astreturnstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/stmt/astreturnstmt.h>

ASTReturnStmt::ASTReturnStmt(std::shared_ptr<ASTExpNode> exp)
    : exp(exp)
{
}

ASTReturnStmt::~ASTReturnStmt() {
}

std::shared_ptr<ASTExpNode> ASTReturnStmt::getExp() {
    return exp;
}

void ASTReturnStmt::print(std::ostream & ss) {
    ss << "return(";
    exp->print(ss);
    ss << ")";
}
