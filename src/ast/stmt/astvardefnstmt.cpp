/**
 * @file astvardefnstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/stmt/astvardefnstmt.h>

ASTVarDefnStmt::ASTVarDefnStmt(std::string id, std::shared_ptr<ASTExpNode> exp)
    : id(id),
      exp(exp)
{
}

ASTVarDefnStmt::~ASTVarDefnStmt() {
}

std::string ASTVarDefnStmt::getId() {
    return id;
}

std::shared_ptr<ASTExpNode> ASTVarDefnStmt::getExp() {
    return exp;
}

void ASTVarDefnStmt::print(std::ostream & ss) {
    ss << "defn(" << id << ", ";
    exp->print(ss);
    ss << ")";
}
