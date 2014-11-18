/**
 * @file astreturnstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astreturnstmt.h>

ASTReturnStmt::ASTReturnStmt(ASTExpNode *exp)
    : exp(exp)
{
}

ASTReturnStmt::~ASTReturnStmt() {
    if (exp)
        delete exp;
}

void ASTReturnStmt::codegen(CodegenCtx *ctx) {
    IRBuilder<> *builder = ctx->getBuilder();

    // For device code, kernel's must return void. So, we inject an extra first
    // argument where the result will be stored.

    if (exp) {
        Value *value = exp->codegen(ctx);

        if (ctx->getEmitDevice()) {
            Value *out_arg = ctx->getFunction()->arg_begin();
            builder->CreateStore(value, out_arg);
            builder->CreateRet(NULL);
        }
        else
            builder->CreateRet(value);
    }
    else
        ctx->getBuilder()->CreateRet(NULL);
}

ASTExpNode *ASTReturnStmt::getExp() {
    return exp;
}
