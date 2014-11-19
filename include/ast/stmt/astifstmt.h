/**
 * @file astifstmt.h
 *
 * @brief Abstract syntax tree 'if' statement node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTIFSTMTNODE_H
#define __ASTIFSTMTNODE_H

#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astexpnode.h>

class ASTIfStmt : public ASTStmtNode {
private:

    ASTExpNode *cond;
    ASTStmtSeqNode *trueStmt;
    ASTStmtSeqNode *falseStmt;

public:

    ASTIfStmt(ASTExpNode *cond, ASTStmtSeqNode *trueStmt, ASTStmtSeqNode *falseStmt);

    ~ASTIfStmt();

    ASTExpNode *getCond();

    ASTStmtSeqNode *getTrueStmt();

    ASTStmtSeqNode *getFalseStmt();

    void print(std::ostream & ss) override;

};

#endif
