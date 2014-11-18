/**
 * @file astvardeclstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astvardeclstmt.h>

ASTVarDeclStmt::ASTVarDeclStmt(ASTType *type, std::string id, ASTExpNode *exp)
    : type(type),
      id(id),
      exp(exp)
{
}

ASTVarDeclStmt::~ASTVarDeclStmt() {
    if (exp)
        delete exp;

    delete type;
}

std::string ASTVarDeclStmt::getId() {
    return id;
}

ASTExpNode *ASTVarDeclStmt::getExp() {
    return exp;
}

ASTType *ASTVarDeclStmt::getType() {
    return type;
}
