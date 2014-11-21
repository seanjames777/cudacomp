/**
 * @file functioninfo.h
 *
 * @brief Information about a function, including its signature and local
 * variable type information.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __FUNCTIONINFO_H
#define __FUNCTIONINFO_H

#include <statics/symboltable.h>
#include <ast/type/asttypenode.h>
#include <ast/type/astfuntype.h>

class FunctionInfo {
private:

    ASTFunType *signature;
    SymbolTable<ASTTypeNode *> locals; // including arguments
    std::string name;

public:

    FunctionInfo(std::string name, ASTFunType *signature);

    ASTFunType *getSignature();

    std::string getName();

    ASTTypeNode *getLocalType(std::string id);

    void addLocal(std::string id, ASTTypeNode *type);

    bool hasLocal(std::string id);

};

#endif
