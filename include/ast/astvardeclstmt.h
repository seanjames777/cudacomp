/**
 * @file astvardeclstmt.h
 *
 * @brief Abstract syntax tree variable declaration statement node. Declares
 * and optionally defines a new variable.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTVARDECLSTMT_H
#define __ASTVARDECLSTMT_H

#include <ast/aststmtnode.h>
#include <ast/astexpnode.h>
#include <ast/asttype.h>

class ASTVarDeclStmt : public ASTStmtNode {
private:

    std::string id;
    ASTExpNode *exp;
    ASTType *type;

public:

    ASTVarDeclStmt(ASTType *type, std::string id, ASTExpNode *exp);

    ~ASTVarDeclStmt();

    std::string getId();

    ASTExpNode *getExp();

    ASTType *getType();

};

#endif
