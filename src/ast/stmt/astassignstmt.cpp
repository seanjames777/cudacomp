/**
 * @file astassignstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/stmt/astassignstmt.h>

ASTAssignStmt::ASTAssignStmt(std::shared_ptr<ASTExpNode> lvalue, std::shared_ptr<ASTExpNode> exp)
    : lvalue(lvalue),
      exp(exp)
{
}

ASTAssignStmt::~ASTAssignStmt() {
}

std::shared_ptr<ASTExpNode> ASTAssignStmt::getLValue() {
    return lvalue;
}

std::shared_ptr<ASTExpNode> ASTAssignStmt::getExp() {
    return exp;
}

void ASTAssignStmt::print(std::ostream & ss) {
    /*ss << "defn(" << id << ", ";
    exp->print(ss);
    ss << ")";*/
    // TODO
}
