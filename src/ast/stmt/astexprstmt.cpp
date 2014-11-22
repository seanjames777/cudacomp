/**
 * @file astexprstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/stmt/astexprstmt.h>

ASTExprStmt::ASTExprStmt(std::shared_ptr<ASTExpNode> exp)
    : exp(exp)
{
}

ASTExprStmt::~ASTExprStmt() {
}

std::shared_ptr<ASTExpNode> ASTExprStmt::getExp() {
    return exp;
}

void ASTExprStmt::print(std::ostream & ss) {
    ss << "expr(";
    exp->print(ss);
    ss << ")";
}
