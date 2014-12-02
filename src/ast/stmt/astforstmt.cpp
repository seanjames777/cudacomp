/**
 * @file astforstmt.cpp
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#include <ast/stmt/astforstmt.h>

ASTForStmt::ASTForStmt(std::shared_ptr<ASTStmtNode> init, std::shared_ptr<ASTExpNode> cond,
           std::shared_ptr<ASTStmtNode> iter, std::shared_ptr<ASTStmtSeqNode> bodyStmt) 
    : init(init),
      cond(cond),
      iter(iter),
      bodyStmt(bodyStmt)
{
}

ASTForStmt::~ASTForStmt() {
}

std::shared_ptr<ASTStmtNode> ASTForStmt::getInit() {
    return init;
}


std::shared_ptr<ASTExpNode> ASTForStmt::getCond() {
    return cond;
}

std::shared_ptr<ASTStmtNode> ASTForStmt::getIter() {
    return iter;
}


std::shared_ptr<ASTStmtSeqNode> ASTForStmt::getBody() {
    return bodyStmt;
}

void ASTForStmt::print(std::ostream & ss) {
}
