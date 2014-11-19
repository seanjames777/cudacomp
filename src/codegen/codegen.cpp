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
#include <ast/astunop.h>
#include <ast/astscope.h>
#include <ast/astifstmt.h>
#include <ast/astboolean.h>
#include <ast/astnopstmt.h>

namespace Codegen {

Value *codegen_exp(CodegenCtx *ctx, ASTExpNode *node) {
    IRBuilder<> *builder = ctx->getBuilder();

    if (ASTInteger *int_exp = dynamic_cast<ASTInteger *>(node))
        return ConstantInt::get(Type::getInt32Ty(ctx->getContext()), int_exp->getValue());
    else if (ASTBoolean *bool_exp = dynamic_cast<ASTBoolean *>(node))
        return ConstantInt::get(Type::getInt1Ty(ctx->getContext()), (int)bool_exp->getValue());
    else if (ASTUnop *unop_exp = dynamic_cast<ASTUnop *>(node)) {
        Value *v = codegen_exp(ctx, unop_exp->getExp());

        switch(unop_exp->getOp()) {
        case ASTUnop::NOT:  return builder->CreateBinOp(Instruction::Xor, v, v);
        case ASTUnop::BNOT: return builder->CreateNot(v);
        case ASTUnop::NEG:  return builder->CreateNeg(v);
        }
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
    else
        throw new ASTMalformedException();

    return NULL;
}

void codegen_stmt(CodegenCtx *ctx, ASTStmtNode *node) {
    IRBuilder<> *builder = ctx->getBuilder();

    // Sequence node
    if (ASTSeqNode *seq_node = dynamic_cast<ASTSeqNode *>(node)) {
        // We always have a head
        ASTStmtNode *head = seq_node->getHead();

        // Return instruction
        if (ASTReturnStmt *ret_node = dynamic_cast<ASTReturnStmt *>(head)) {
            ASTExpNode *ret_exp = ret_node->getExp();

            // Need to return a value
            if (ret_exp) {
                Value *ret_val = codegen_exp(ctx, ret_exp);

                // In device mode, have to move into return value argument because kernels
                // must return void
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

            // Don't keep generating code because we've returned and we can't add a basic block
            // after the return anyway.
            return;
        }
        // Variable declaration
        else if (ASTVarDeclStmt *decl_stmt = dynamic_cast<ASTVarDeclStmt *>(head)) {
            // Creates an alloca. TODO maybe getOrCreateId()
            Value *mem = ctx->getSymbol(decl_stmt->getId());

            if (decl_stmt->getExp()) {
                Value *exp_val = codegen_exp(ctx, decl_stmt->getExp());
                builder->CreateStore(exp_val, mem);
            }
        }
        // Variable definution
        else if (ASTVarDefnStmt *decl_stmt = dynamic_cast<ASTVarDefnStmt *>(head)) {
            Value *mem = ctx->getSymbol(decl_stmt->getId());
            Value *exp_val = codegen_exp(ctx, decl_stmt->getExp());
            builder->CreateStore(exp_val, mem);
        }
        // Scope
        else if (ASTScope *scope_stmt = dynamic_cast<ASTScope *>(head)) {
            if (scope_stmt->getBody())
                codegen_stmt(ctx, scope_stmt->getBody());
        }
        else if (ASTIfStmt *if_node = dynamic_cast<ASTIfStmt *>(head)) {
            Value *cond = codegen_exp(ctx, if_node->getCond());

            BasicBlock *trueBlock = ctx->createBlock();
            BasicBlock *falseBlock = ctx->createBlock();
            BasicBlock *doneBlock = ctx->createBlock();

            // Generate conditional jump
            ctx->getBuilder()->CreateCondBr(cond, trueBlock, falseBlock);

            // Generate 'true' branch
            ctx->pushBlock(trueBlock);
            codegen_stmt(ctx, if_node->getTrueStmt());
            ctx->getBuilder()->CreateBr(doneBlock);

            // Generate 'false' branch
            ctx->pushBlock(falseBlock);
            // TODO check if it optimizes falseBlock -> doneBlock
            if (if_node->getFalseStmt())
                codegen_stmt(ctx, if_node->getFalseStmt());
            ctx->getBuilder()->CreateBr(doneBlock);

            // 'doneBlock' remains on the stack
            ctx->pushBlock(doneBlock);
        }
        else if (ASTNopStmt *nop_node = dynamic_cast<ASTNopStmt *>(head)) {
            // Nothing to do
        }
        else
            throw new ASTMalformedException();

        if (seq_node->getTail())
            codegen_stmt(ctx, seq_node->getTail());
    }
    // Should always be a null terminated linked list
    else
        throw new ASTMalformedException();
}

}
