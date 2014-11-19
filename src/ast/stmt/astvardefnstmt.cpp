/**
 * @file astvardefnstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astvardefnstmt.h>

ASTVarDefnStmt::ASTVarDefnStmt(std::string id, ASTExpNode *exp)
    : id(id),
      exp(exp)
{
}

ASTVarDefnStmt::~ASTVarDefnStmt() {
    delete exp;
}

std::string ASTVarDefnStmt::getId() {
    return id;
}

ASTExpNode *ASTVarDefnStmt::getExp() {
    return exp;
}

void ASTVarDefnStmt::print(std::ostream & ss) {
    ss << "defn(" << id << ", ";
    exp->print(ss);
    ss << ")";
}
