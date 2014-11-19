/**
 * @file typectx.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/typectx.h>
#include <ast/astintegertype.h>

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

void TypeCtx::setSymbol(std::string id, ASTType *type) {
    symbols[id] = type;
}

Type *TypeCtx::getSymbol(std::string id) {
    return convert_type(symbols[id]);
}

ASTType *TypeCtx::getSymbolAST(std::string id) {
    return symbols[id];
}

Type *TypeCtx::convert_type(ASTType *type) {
    if (ASTIntegerType *int_type = dynamic_cast<ASTIntegerType *>(type))
        return Type::getInt32Ty(context);

    throw new ASTMalformedException();
}
