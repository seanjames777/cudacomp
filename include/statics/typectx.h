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
#include <ast/type/asttype.h>

class TypeCtx {
private:

    std::unordered_map<std::string, ASTType *> symbols;

public:

    TypeCtx();

    void setSymbol(std::string id, ASTType *type);

    ASTType *getSymbol(std::string id);

};

#endif
