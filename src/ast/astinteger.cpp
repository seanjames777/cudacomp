/**
 * @file astinteger.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astinteger.h>

ASTInteger::ASTInteger(int value)
    : value(value)
{
}

ASTInteger::~ASTInteger() {
}

int ASTInteger::getValue() {
    return value;
}

Value *ASTInteger::codegen(CodegenCtx *ctx) {
    Type *type = Type::getInt32Ty(ctx->getContext());
    return ConstantInt::get(type, value);
}
