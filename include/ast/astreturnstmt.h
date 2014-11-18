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

#include <ast/aststmtnode.h>
#include <ast/astexpnode.h>

class ASTReturnStmt : public ASTStmtNode {
private:

    ASTExpNode *exp;

public:

    ASTReturnStmt(ASTExpNode *exp);

    ~ASTReturnStmt();

    ASTExpNode *getExp();

};

#endif
