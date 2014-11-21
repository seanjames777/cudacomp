/**
 * @file astwhilestmt.cpp
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#include <ast/stmt/astwhilestmt.h>

ASTWhileStmt::ASTWhileStmt(ASTExpNode *cond, ASTStmtSeqNode *bodyStmt)
    : cond(cond),
      bodyStmt(bodyStmt)
{
}

ASTWhileStmt::~ASTWhileStmt() {
    delete cond;
    delete bodyStmt;
}

ASTExpNode *ASTWhileStmt::getCond() {
    return cond;
}

ASTStmtSeqNode *ASTWhileStmt::getBodyStmt() {
    return bodyStmt;
}

void ASTWhileStmt::print(std::ostream & ss) {
    ss << "while(";
    cond->print(ss);
    ss << "," << std::endl;
    bodyStmt->print(ss);
    ss << ")";

}
