/**
 * @file astifstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astifstmt.h>

ASTIfStmt::ASTIfStmt(ASTExpNode *cond, ASTStmtNode *trueStmt, ASTStmtNode *falseStmt)
    : cond(cond),
      trueStmt(trueStmt),
      falseStmt(falseStmt)
{
}

ASTIfStmt::~ASTIfStmt() {
    delete cond;
    delete trueStmt;

    if (falseStmt)
        delete falseStmt;
}

ASTExpNode *ASTIfStmt::getCond() {
    return cond;
}

ASTStmtNode *ASTIfStmt::getTrueStmt() {
    return trueStmt;
}

ASTStmtNode *ASTIfStmt::getFalseStmt() {
    return falseStmt;
}
