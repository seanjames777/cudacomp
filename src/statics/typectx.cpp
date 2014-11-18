/**
 * @file typectx.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/typectx.h>

TypeCtx::TypeCtx()
    : context(getGlobalContext())
{
}

LLVMContext & TypeCtx::getContext() {
    return context;
}

void TypeCtx::setType(ASTExpNode *exp, Type *type) {
    types[exp] = type;
}

Type *TypeCtx::getType(ASTExpNode *exp) {
    return types[exp];
}

void TypeCtx::setSymbol(std::string id, Type *type) {
    symbols[id] = type;
}

Type *TypeCtx::getSymbol(std::string id) {
    return symbols[id];
}
