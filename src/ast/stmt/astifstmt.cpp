/**
 * @file astifstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/stmt/astifstmt.h>

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

void ASTIfStmt::print(std::ostream & ss) {
    ss << "if(";
    cond->print(ss);
    ss << "," << std::endl;
    trueStmt->print(ss);
    ss << "," << std::endl;

    if (falseStmt)
        falseStmt->print(ss);
    else
        ss << "null";

    ss << ")";

}
