/**
 * @file astifstmt.h
 *
 * @brief Abstract syntax tree 'if' statement node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTIFSTMTNODE_H
#define __ASTIFSTMTNODE_H

#include <ast/aststmtnode.h>
#include <ast/astexpnode.h>

class ASTIfStmt : public ASTStmtNode {
private:

    ASTExpNode *cond;
    ASTStmtNode *trueStmt;
    ASTStmtNode *falseStmt;

public:

    ASTIfStmt(ASTExpNode *cond, ASTStmtNode *trueStmt, ASTStmtNode *falseStmt);

    ~ASTIfStmt();

    ASTExpNode *getCond();

    ASTStmtNode *getTrueStmt();

    ASTStmtNode *getFalseStmt();

};

#endif
