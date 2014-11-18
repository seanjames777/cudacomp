/**
 * @file astreturnstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astreturnstmt.h>

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
