/**
 * @file astassignstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/stmt/astassignstmt.h>

ASTAssignStmt::ASTAssignStmt(
    enum ASTBinopExp::op op,
    std::shared_ptr<ASTExpNode> lvalue,
    std::shared_ptr<ASTExpNode> exp)
    : op(op),
      lvalue(lvalue),
      exp(exp),
      type(nullptr)
{
}

ASTAssignStmt::~ASTAssignStmt() {
}

enum ASTBinopExp::op ASTAssignStmt::getOp() {
    return op;
}

std::shared_ptr<ASTExpNode> ASTAssignStmt::getLValue() {
    return lvalue;
}

std::shared_ptr<ASTExpNode> ASTAssignStmt::getExp() {
    return exp;
}

std::shared_ptr<ASTTypeNode> ASTAssignStmt::getType() {
    return type;
}

void ASTAssignStmt::setType(std::shared_ptr<ASTTypeNode> type) {
    this->type = type;
}

void ASTAssignStmt::print(std::ostream & ss) {
}
