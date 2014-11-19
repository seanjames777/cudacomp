/**
 * @file astvardefnstmt.h
 *
 * @brief Abstract syntax tree variable definition statement node.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTVARDEFNSTMT_H
#define __ASTVARDEFNSTMT_H

#include <ast/aststmtnode.h>
#include <ast/astexpnode.h>

class ASTVarDefnStmt : public ASTStmtNode {
private:

    std::string id;
    ASTExpNode *exp;

public:

    ASTVarDefnStmt(std::string id, ASTExpNode *exp);

    ~ASTVarDefnStmt();

    std::string getId();

    ASTExpNode *getExp();

    void print(std::ostream & ss) override;

};

#endif
