/**
 * @file typectx.h
 *
 * @brief Type checking context
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __TYPECTX_H
#define __TYPECTX_H

#include <defs.h>
#include <ast/expr/astexpnode.h>
#include <ast/type/asttype.h>

class TypeCtx {
private:

    LLVMContext & context;

    std::unordered_map<std::string, ASTType *> symbols;
    std::unordered_map<ASTExpNode *, Type *> types;

public:

    TypeCtx();

    LLVMContext & getContext();

    void setType(ASTExpNode *exp, Type *type);

    Type *getType(ASTExpNode *exp);

    void setSymbol(std::string id, ASTType *type);

    Type *getSymbol(std::string id);

    ASTType *getSymbolAST(std::string id);

    Type *convert_type(ASTType *type);

};

#endif
