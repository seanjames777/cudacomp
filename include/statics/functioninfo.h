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
#include <ast/type/asttype.h>
#include <ast/type/astfuntype.h>

class FunctionInfo {
private:

    ASTFunType *signature;
    SymbolTable<ASTType *> locals; // including arguments
    std::string name;

public:

    FunctionInfo(std::string name, ASTFunType *signature);

    ASTFunType *getSignature();

    std::string getName();

    ASTType *getLocalType(std::string id);

    void addLocal(std::string id, ASTType *type);

    bool hasLocal(std::string id);

};

#endif
