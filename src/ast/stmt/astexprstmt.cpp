/**
 * @file astexprstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/stmt/astexprstmt.h>

ASTExprStmt::ASTExprStmt(ASTExpNode *exp)
    : exp(exp)
{
}

ASTExprStmt::~ASTExprStmt() {
    if (exp)
        delete exp;
}

ASTExpNode *ASTExprStmt::getExp() {
    return exp;
}

void ASTExprStmt::print(std::ostream & ss) {
    ss << "expr(";
    exp->print(ss);
    ss << ")";
}
