/**
 * @file astexprstmt.h
 *
 * @brief Abstract syntax tree expression statement node. Simply a wrapper that
 * execute an expression for its side effects without storing the value.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTEXPRSTMT_H
#define __ASTEXPRSTMT_H

#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astexpnode.h>

class ASTExprStmt : public ASTStmtNode {
private:

    ASTExpNode *exp;

public:

    ASTExprStmt(ASTExpNode *exp);

    ~ASTExprStmt();

    ASTExpNode *getExp();

    void print(std::ostream & ss) override;

};

#endif
