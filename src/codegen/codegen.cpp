/**
 * @file codegen.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <codegen/codegen.h>
#include <ast/expr/astidentifier.h>
#include <ast/expr/astinteger.h>
#include <ast/expr/astbinop.h>
#include <ast/astseqnode.h>
#include <ast/stmt/astreturnstmt.h>
#include <ast/stmt/astvardeclstmt.h>
#include <ast/stmt/astvardefnstmt.h>
#include <ast/expr/astunop.h>
#include <ast/stmt/astscope.h>
#include <ast/stmt/astifstmt.h>
#include <ast/expr/astboolean.h>
#include <ast/type/astintegertype.h>
#include <ast/type/astbooleantype.h>
#include <codegen/converttype.h>
#include <ast/expr/astcall.h>

namespace Codegen {

Value *codegen_exp(CodegenCtx *ctx, ASTExpNode *node) {
    IRBuilder<> *builder = ctx->getBuilder();

    // Integer constant
    if (ASTInteger *int_exp = dynamic_cast<ASTInteger *>(node))
        return ConstantInt::get(convertType(ASTIntegerType::get()), int_exp->getValue());
    // Boolean constant
    else if (ASTBoolean *bool_exp = dynamic_cast<ASTBoolean *>(node))
        return ConstantInt::get(convertType(ASTBooleanType::get()), (int)bool_exp->getValue());
    // Unary operator
    else if (ASTUnop *unop_exp = dynamic_cast<ASTUnop *>(node)) {
        Value *v = codegen_exp(ctx, unop_exp->getExp());

        switch(unop_exp->getOp()) {
        case ASTUnop::NOT:  return builder->CreateBinOp(Instruction::Xor, v, v);
        case ASTUnop::BNOT: return builder->CreateNot(v);
        case ASTUnop::NEG:  return builder->CreateNeg(v);
        }
    }
    // Binary operator
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
        case ASTBinop::EQ:   return builder->CreateICmpEQ(v1, v2);
        case ASTBinop::NEQ:  return builder->CreateICmpNE(v1, v2);
        case ASTBinop::LEQ:  return builder->CreateICmpSLE(v1, v2);
        case ASTBinop::GEQ:  return builder->CreateICmpSGE(v1, v2);
        case ASTBinop::LT:   return builder->CreateICmpSLT(v1, v2);
        case ASTBinop::GT:   return builder->CreateICmpSGT(v1, v2);
        }
    }
    // Identifier reference
    else if (ASTIdentifier *id_exp = dynamic_cast<ASTIdentifier *>(node)) {
        Value *id_ptr = ctx->getOrCreateSymbol(id_exp->getId());
        return builder->CreateLoad(id_ptr);
    }
    // Function call
    else if (ASTCall *call_exp = dynamic_cast<ASTCall *>(node)) {
        std::vector<Value *> args;

        Value *ret_val = NULL;

        ASTFunType *funDefn = ctx->getModuleInfo()->getFunction(call_exp->getId())->getSignature();

        // In device mode, add a pointer to a new temp to get the return value
        if (ctx->getEmitDevice()) {
            // TODO use an address of instead maybe
            ret_val = builder->CreateAlloca(convertType(funDefn->getReturnType()));
            args.push_back(ret_val);
        }

        ASTExpSeqNode *exp_args = call_exp->getArgs();

        // Codegen each argument
        while (exp_args != NULL) {
            ASTExpNode *arg = exp_args->getHead();
            args.push_back(codegen_exp(ctx, arg));
            exp_args = exp_args->getTail();
        }

        Value *call = builder->CreateCall(ctx->getFunction(call_exp->getId()), args);

        if (!ctx->getEmitDevice())
            ret_val = call;
        else {
            ret_val = builder->CreateLoad(ret_val);
        }

        return ret_val;
    }
    else
        throw new ASTMalformedException();

    return NULL;
}

void codegen_stmts(CodegenCtx *ctx, ASTStmtSeqNode *seq_node) {
    while (seq_node != NULL) {
        if (!codegen_stmt(ctx, seq_node->getHead()))
            break;
        seq_node = seq_node->getTail();
    }
}

bool codegen_stmt(CodegenCtx *ctx, ASTStmtNode *head) {
    IRBuilder<> *builder = ctx->getBuilder();

    // Return instruction
    if (ASTReturnStmt *ret_node = dynamic_cast<ASTReturnStmt *>(head)) {
        ASTExpNode *ret_exp = ret_node->getExp();

        // Need to return a value
        if (ret_exp) {
            Value *ret_val = codegen_exp(ctx, ret_exp);

            // In device mode, have to move into return value argument because kernels
            // must return void
            if (ctx->getEmitDevice()) {
                Value *out_arg = ctx->getCurrentFunction()->arg_begin();

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
        return false;
    }
    // Variable declaration
    else if (ASTVarDeclStmt *decl_stmt = dynamic_cast<ASTVarDeclStmt *>(head)) {
        // Creates an alloca. TODO maybe getOrCreateId()
        Value *mem = ctx->getOrCreateSymbol(decl_stmt->getId());

        if (decl_stmt->getExp()) {
            Value *exp_val = codegen_exp(ctx, decl_stmt->getExp());
            builder->CreateStore(exp_val, mem);
        }
    }
    // Variable definution
    else if (ASTVarDefnStmt *decl_stmt = dynamic_cast<ASTVarDefnStmt *>(head)) {
        Value *mem = ctx->getOrCreateSymbol(decl_stmt->getId());
        Value *exp_val = codegen_exp(ctx, decl_stmt->getExp());
        builder->CreateStore(exp_val, mem);
    }
    // Scope
    else if (ASTScope *scope_stmt = dynamic_cast<ASTScope *>(head)) {
        if (scope_stmt->getBody())
            codegen_stmts(ctx, scope_stmt->getBody());
    }
    // If statement
    else if (ASTIfStmt *if_node = dynamic_cast<ASTIfStmt *>(head)) {
        Value *cond = codegen_exp(ctx, if_node->getCond());

        BasicBlock *trueBlock = ctx->createBlock();
        BasicBlock *falseBlock = ctx->createBlock();
        BasicBlock *doneBlock = ctx->createBlock();

        // Generate conditional jump
        ctx->getBuilder()->CreateCondBr(cond, trueBlock, falseBlock);

        // Generate 'true' branch
        ctx->pushBlock(trueBlock);
        codegen_stmts(ctx, if_node->getTrueStmt());
        ctx->getBuilder()->CreateBr(doneBlock);

        // Generate 'false' branch
        ctx->pushBlock(falseBlock);

        if (if_node->getFalseStmt())
            codegen_stmts(ctx, if_node->getFalseStmt());

        ctx->getBuilder()->CreateBr(doneBlock);

        // 'doneBlock' remains on the stack
        ctx->pushBlock(doneBlock);

        // TODO: check if this is making phi nodes
    }
    else
        throw new ASTMalformedException();

    return true;
}

void codegen_tops(ModuleInfo *module, ASTTopSeqNode *nodes, bool emitDevice, std::ostream & out) {
    CodegenCtx ctx(emitDevice, module);

    ASTTopSeqNode *node = nodes;

    while (node != NULL) {
        ASTTopNode *top_node = node->getHead();

        // Create LLVM functions for each function
        if (ASTFunDefn *funDefn = dynamic_cast<ASTFunDefn *>(top_node)) {
            FunctionInfo *funInfo = module->getFunction(funDefn->getName());
            ctx.createFunction(funInfo);
        }

        node = node->getTail();
    }

    node = nodes;

    while (node != NULL) {
        codegen_top(&ctx, node->getHead());
        node = node->getTail();
    }

    ctx.emit(out);
}

void codegen_top(CodegenCtx *ctx, ASTTopNode *node) {
    if (ASTFunDefn *funDefn = dynamic_cast<ASTFunDefn *>(node)) {
        FunctionInfo *func = ctx->getModuleInfo()->getFunction(funDefn->getName());

        ctx->startFunction(func->getName());
        codegen_stmts(ctx, funDefn->getBody());

        if (ctx->getEmitDevice() && func->getName() == "_cc_main") // TODO require this function
            ctx->markKernel(ctx->getFunction(func->getName()));

        ctx->finishFunction();
    }
    else
        throw new ASTMalformedException();
}

}

// TODO: make the codegen context over the whole module and add code to push functions
// TODO: do something similar for type checking and other analyses
