/**
 * @file astifstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/stmt/astifstmt.h>

ASTIfStmt::ASTIfStmt(std::shared_ptr<ASTExpNode> cond, std::shared_ptr<ASTStmtSeqNode> trueStmt, std::shared_ptr<ASTStmtSeqNode> falseStmt)
    : cond(cond),
      trueStmt(trueStmt),
      falseStmt(falseStmt)
{
}

ASTIfStmt::~ASTIfStmt() {
}

std::shared_ptr<ASTExpNode> ASTIfStmt::getCond() {
    return cond;
}

std::shared_ptr<ASTStmtSeqNode> ASTIfStmt::getTrueStmt() {
    return trueStmt;
}

std::shared_ptr<ASTStmtSeqNode> ASTIfStmt::getFalseStmt() {
    return falseStmt;
}

void ASTIfStmt::print(std::ostream & ss) {
}
