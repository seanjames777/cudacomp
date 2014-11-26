/**
 * @file astwhilestmt.cpp
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#include <ast/stmt/astwhilestmt.h>

ASTWhileStmt::ASTWhileStmt(std::shared_ptr<ASTExpNode> cond, std::shared_ptr<ASTStmtSeqNode> bodyStmt)
    : cond(cond),
      bodyStmt(bodyStmt)
{
}

ASTWhileStmt::~ASTWhileStmt() {
}

std::shared_ptr<ASTExpNode> ASTWhileStmt::getCond() {
    return cond;
}

std::shared_ptr<ASTStmtSeqNode> ASTWhileStmt::getBody() {
    return bodyStmt;
}

void ASTWhileStmt::print(std::ostream & ss) {
}
