/**
 * @file astreturnstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/stmt/astreturnstmt.h>

ASTReturnStmt::ASTReturnStmt(ASTExpNode *exp)
    : exp(exp)
{
}

ASTReturnStmt::~ASTReturnStmt() {
    if (exp)
        delete exp;
}

ASTExpNode *ASTReturnStmt::getExp() {
    return exp;
}

void ASTReturnStmt::print(std::ostream & ss) {
    ss << "return(";
    exp->print(ss);
    ss << ")";
}
