/**
 * @file astwhilestmt.h
 *
 * @brief Abstract syntax tree 'while' statement node
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#ifndef __ASTWHILESTMTNODE_H
#define __ASTWHILESTMTNODE_H

#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astexpnode.h>

class ASTWhileStmt : public ASTStmtNode {
private:

    ASTExpNode *cond;
    ASTStmtSeqNode *bodyStmt;

public:

    ASTWhileStmt(ASTExpNode *cond, ASTStmtSeqNode *bodyStmt);

    ~ASTWhileStmt();

    ASTExpNode *getCond();

    ASTStmtSeqNode *getBodyStmt();

    void print(std::ostream & ss) override;

};

#endif
