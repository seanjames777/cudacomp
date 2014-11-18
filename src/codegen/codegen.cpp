/**
 * @file codegen.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <codegen/codegen.h>
#include <ast/astidentifier.h>
#include <ast/astinteger.h>
#include <ast/astbinop.h>
#include <ast/astseqnode.h>
#include <ast/astreturnstmt.h>
#include <ast/astvardeclstmt.h>
#include <ast/astvardefnstmt.h>

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
        case ASTBinop::ADD:  return builder->CreateBinOp(Instruction::Add, v1, v2);
        case ASTBinop::SUB:  return builder->CreateBinOp(Instruction::Sub, v1, v2);
        case ASTBinop::MUL:  return builder->CreateBinOp(Instruction::Mul, v1, v2);
        case ASTBinop::DIV:  return builder->CreateBinOp(Instruction::SDiv, v1, v2);
        case ASTBinop::MOD:  return builder->CreateBinOp(Instruction::SRem, v1, v2);
        case ASTBinop::SHL:  return builder->CreateBinOp(Instruction::Shl, v1, v2);
        case ASTBinop::SHR:  return builder->CreateBinOp(Instruction::AShr, v1, v2);
        case ASTBinop::AND:  return builder->CreateBinOp(Instruction::And, v1, v2);
        case ASTBinop::OR:   return builder->CreateBinOp(Instruction::Or, v1, v2);
        case ASTBinop::BAND: return builder->CreateBinOp(Instruction::And, v1, v2);
        case ASTBinop::BOR:  return builder->CreateBinOp(Instruction::Or, v1, v2);
        case ASTBinop::BXOR: return builder->CreateBinOp(Instruction::Xor, v1, v2);
        }
    }
    else if (ASTIdentifier *id_exp = dynamic_cast<ASTIdentifier *>(node)) {
        Value *id_ptr = ctx->getSymbol(id_exp->getId());
        return builder->CreateLoad(id_ptr);
    }
    else {
        throw new ASTMalformedException();
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
    else if (ASTVarDeclStmt *decl_stmt = dynamic_cast<ASTVarDeclStmt *>(node)) {
        // Creates an alloca. TODO maybe getOrCreateId()
        Value *mem = ctx->getSymbol(decl_stmt->getId());

        if (decl_stmt->getExp()) {
            Value *exp_val = codegen_exp(ctx, decl_stmt->getExp());
            builder->CreateStore(exp_val, mem);
        }
    }
    else if (ASTVarDefnStmt *decl_stmt = dynamic_cast<ASTVarDefnStmt *>(node)) {
        Value *mem = ctx->getSymbol(decl_stmt->getId());
        Value *exp_val = codegen_exp(ctx, decl_stmt->getExp());
        builder->CreateStore(exp_val, mem);
    }
    else {
        throw new ASTMalformedException();
    }
}

}
