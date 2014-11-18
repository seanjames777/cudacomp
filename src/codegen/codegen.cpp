/**
 * @file codegen.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <codegen/codegen.h>
#include <ast/astinteger.h>
#include <ast/astbinop.h>
#include <ast/astseqnode.h>
#include <ast/astreturnstmt.h>

namespace Codegen {

Value *codegen_exp(CodegenCtx *ctx, ASTExpNode *node) {
    IRBuilder<> *builder = ctx->getBuilder();

    if (ASTInteger *int_exp = dynamic_cast<ASTInteger *>(node)) {
        return ConstantInt::get(Type::getInt32Ty(ctx->getContext()), int_exp->getValue());
    }
    else if (ASTBinop *binop_exp = dynamic_cast<ASTBinop *>(node)) {
        Value *v1 = codegen_exp(ctx, binop_exp->getE1());
        Value *v2 = codegen_exp(ctx, binop_exp->getE2());

        switch(binop_exp->getOp()) {
        case ASTBinop::ADD: return builder->CreateBinOp(Instruction::Add, v1, v2);
        case ASTBinop::SUB: return builder->CreateBinOp(Instruction::Sub, v1, v2);
        case ASTBinop::MUL: return builder->CreateBinOp(Instruction::Mul, v1, v2);
        case ASTBinop::DIV: return builder->CreateBinOp(Instruction::SDiv, v1, v2);
        }
    }

    return NULL;
}

void codegen_stmt(CodegenCtx *ctx, ASTStmtNode *node) {
    IRBuilder<> *builder = ctx->getBuilder();

    if (ASTSeqNode *seq_node = dynamic_cast<ASTSeqNode *>(node)) {
        codegen_stmt(ctx, seq_node->getHead());

        if (seq_node->getTail())
            codegen_stmt(ctx, seq_node->getTail());
    }
    else if (ASTReturnStmt *ret_node = dynamic_cast<ASTReturnStmt *>(node)) {
        ASTExpNode *ret_exp = ret_node->getExp();

        if (ret_exp) {
            Value *ret_val = codegen_exp(ctx, ret_exp);

            if (ctx->getEmitDevice()) {
                Value *out_arg = ctx->getFunction()->arg_begin();

                builder->CreateStore(ret_val, out_arg);
                builder->CreateRet(NULL);
            }
            else
                builder->CreateRet(ret_val);
        }
        else
            builder->CreateRet(NULL);
    }
}

}
