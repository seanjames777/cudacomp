/**
 * @file astreturnstmt.h
 *
 * @brief Abstract syntax tree return statement node. Returns an expression to
 * the calling function.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTRETURNSTMT_H
#define __ASTRETURNSTMT_H

#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astexpnode.h>

class ASTReturnStmt : public ASTStmtNode {
private:

    ASTExpNode *exp;

public:

    ASTReturnStmt(ASTExpNode *exp);

    ~ASTReturnStmt();

    ASTExpNode *getExp();

    void print(std::ostream & ss) override;

};

#endif
