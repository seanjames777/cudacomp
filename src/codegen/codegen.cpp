/**
 * @file codegen.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <codegen/codegen.h>
#include <ast/expr/astidentifierexp.h>
#include <ast/expr/astintegerexp.h>
#include <ast/expr/astbinopexp.h>
#include <ast/astseqnode.h>
#include <ast/stmt/astreturnstmt.h>
#include <ast/stmt/astvardeclstmt.h>
#include <ast/stmt/astvardefnstmt.h>
#include <ast/expr/astunopexp.h>
#include <ast/stmt/astscopestmt.h>
#include <ast/stmt/astifstmt.h>
#include <ast/expr/astbooleanexp.h>
#include <ast/type/astintegertype.h>
#include <ast/type/astbooleantype.h>
#include <codegen/converttype.h>
#include <ast/expr/astcallexp.h>
#include <ast/type/astvoidtype.h>
#include <ast/stmt/astexprstmt.h>

namespace Codegen {

Value *codegen_exp(CodegenCtx *ctx, std::shared_ptr<ASTExpNode> node) {
    IRBuilder<> *builder = ctx->getBuilder();

    // Integer constant
    if (std::shared_ptr<ASTIntegerExp> int_exp = std::dynamic_pointer_cast<ASTIntegerExp>(node))
        return ConstantInt::get(convertType(ASTIntegerType::get()), int_exp->getValue());
    // Boolean constant
    else if (std::shared_ptr<ASTBooleanExp> bool_exp = std::dynamic_pointer_cast<ASTBooleanExp>(node))
        return ConstantInt::get(convertType(ASTBooleanType::get()), (int)bool_exp->getValue());
    // Unary operator
    else if (std::shared_ptr<ASTUnopExp> unop_exp = std::dynamic_pointer_cast<ASTUnopExp>(node)) {
        Value *v = codegen_exp(ctx, unop_exp->getExp());

        switch(unop_exp->getOp()) {
        case ASTUnopExp::NOT:  return builder->CreateBinOp(Instruction::Xor, v, v);
        case ASTUnopExp::BNOT: return builder->CreateNot(v);
        case ASTUnopExp::NEG:  return builder->CreateNeg(v); // TODO investigate x86
        }
    }
    // Binary operator
    else if (std::shared_ptr<ASTBinopExp> binop_exp = std::dynamic_pointer_cast<ASTBinopExp>(node)) {
        Value *v1 = codegen_exp(ctx, binop_exp->getE1());
        Value *v2 = codegen_exp(ctx, binop_exp->getE2());

        switch(binop_exp->getOp()) {
        case ASTBinopExp::ADD:  return builder->CreateBinOp(Instruction::Add, v1, v2);
        case ASTBinopExp::SUB:  return builder->CreateBinOp(Instruction::Sub, v1, v2);
        case ASTBinopExp::MUL:  return builder->CreateBinOp(Instruction::Mul, v1, v2);
        case ASTBinopExp::DIV:  return builder->CreateBinOp(Instruction::SDiv, v1, v2);
        case ASTBinopExp::MOD:  return builder->CreateBinOp(Instruction::SRem, v1, v2);
        case ASTBinopExp::SHL:  return builder->CreateBinOp(Instruction::Shl, v1, v2);
        case ASTBinopExp::SHR:  return builder->CreateBinOp(Instruction::AShr, v1, v2);
        case ASTBinopExp::AND:  return builder->CreateBinOp(Instruction::And, v1, v2);
        case ASTBinopExp::OR:   return builder->CreateBinOp(Instruction::Or, v1, v2);
        case ASTBinopExp::BAND: return builder->CreateBinOp(Instruction::And, v1, v2);
        case ASTBinopExp::BOR:  return builder->CreateBinOp(Instruction::Or, v1, v2);
        case ASTBinopExp::BXOR: return builder->CreateBinOp(Instruction::Xor, v1, v2);
        case ASTBinopExp::EQ:   return builder->CreateICmpEQ(v1, v2);
        case ASTBinopExp::NEQ:  return builder->CreateICmpNE(v1, v2);
        case ASTBinopExp::LEQ:  return builder->CreateICmpSLE(v1, v2);
        case ASTBinopExp::GEQ:  return builder->CreateICmpSGE(v1, v2);
        case ASTBinopExp::LT:   return builder->CreateICmpSLT(v1, v2);
        case ASTBinopExp::GT:   return builder->CreateICmpSGT(v1, v2);
        }
    }
    // Identifier reference
    else if (std::shared_ptr<ASTIdentifierExp> id_exp = std::dynamic_pointer_cast<ASTIdentifierExp>(node)) {
        Value *id_ptr = ctx->getOrCreateSymbol(id_exp->getId());
        return builder->CreateLoad(id_ptr);
    }
    // Function call
    else if (std::shared_ptr<ASTCallExp> call_exp = std::dynamic_pointer_cast<ASTCallExp>(node)) {
        std::vector<Value *> args;

        Value *ret_val = NULL;

        std::shared_ptr<ASTFunType> funDefn = ctx->getModuleInfo()->getFunction(call_exp->getId())->getSignature();
        bool isVoid = funDefn->getReturnType()->equal(ASTVoidType::get());

        // In device mode, add a pointer to a new temp to get the return value
        if (ctx->getEmitDevice() && !isVoid) {
            // TODO use an address of instead maybe
            ret_val = ctx->createTemp(convertType(funDefn->getReturnType()));
            args.push_back(ret_val);
        }

        std::shared_ptr<ASTExpSeqNode> exp_args = call_exp->getArgs();

        // Codegen each argument
        while (exp_args != NULL) {
            std::shared_ptr<ASTExpNode> arg = exp_args->getHead();
            args.push_back(codegen_exp(ctx, arg));
            exp_args = exp_args->getTail();
        }

        Value *call = builder->CreateCall(ctx->getFunction(call_exp->getId()), args);

        if (!ctx->getEmitDevice())
            ret_val = call;
        else if (!isVoid) {
            ret_val = builder->CreateLoad(ret_val);
        }

        // If the function has a void return type, we'll return NULL, but this should have
        // already been handled by the type checker: can't assign a void expression to any
        // type of lvalue.
        return ret_val;
    }
    else
        throw new ASTMalformedException();

    return NULL;
}

bool codegen_stmts(CodegenCtx *ctx, std::shared_ptr<ASTStmtSeqNode> seq_node) {
    while (seq_node != NULL) {
        if (!codegen_stmt(ctx, seq_node->getHead()))
            return false;
        seq_node = seq_node->getTail();
    }

    return true;
}

bool codegen_stmt(CodegenCtx *ctx, std::shared_ptr<ASTStmtNode> head) {
    IRBuilder<> *builder = ctx->getBuilder();

    // Return instruction
    if (std::shared_ptr<ASTReturnStmt> ret_node = std::dynamic_pointer_cast<ASTReturnStmt>(head)) {
        std::shared_ptr<ASTExpNode> ret_exp = ret_node->getExp();

        // Void expressions don't need to return a value
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
    else if (std::shared_ptr<ASTVarDeclStmt> decl_stmt = std::dynamic_pointer_cast<ASTVarDeclStmt>(head)) {
        // Creates an alloca. TODO maybe getOrCreateId()
        Value *mem = ctx->getOrCreateSymbol(decl_stmt->getId());

        if (decl_stmt->getExp()) {
            Value *exp_val = codegen_exp(ctx, decl_stmt->getExp());
            builder->CreateStore(exp_val, mem);
        }
    }
    // Variable definution
    else if (std::shared_ptr<ASTVarDefnStmt> decl_stmt = std::dynamic_pointer_cast<ASTVarDefnStmt>(head)) {
        Value *mem = ctx->getOrCreateSymbol(decl_stmt->getId());
        Value *exp_val = codegen_exp(ctx, decl_stmt->getExp());
        builder->CreateStore(exp_val, mem);
    }
    // Scope
    else if (std::shared_ptr<ASTScopeStmt> scope_stmt = std::dynamic_pointer_cast<ASTScopeStmt>(head)) {
        if (scope_stmt->getBody())
            return codegen_stmts(ctx, scope_stmt->getBody());
    }
    // If statement
    else if (std::shared_ptr<ASTIfStmt> if_node = std::dynamic_pointer_cast<ASTIfStmt>(head)) {
        Value *cond = codegen_exp(ctx, if_node->getCond());

        BasicBlock *trueBlock = ctx->createBlock();
        BasicBlock *falseBlock = ctx->createBlock();
        BasicBlock *doneBlock = ctx->createBlock();

        // Generate conditional jump
        ctx->getBuilder()->CreateCondBr(cond, trueBlock, falseBlock);

        // Generate 'true' branch
        ctx->pushBlock(trueBlock);

        // Whether the left and right branches did not return
        bool leftContinue = true;
        bool rightContinue = true;

        // Only need to insert the jump if the statement didn't return
        if ((leftContinue = codegen_stmts(ctx, if_node->getTrueStmt())))
            ctx->getBuilder()->CreateBr(doneBlock);

        // Generate 'false' branch
        ctx->pushBlock(falseBlock);

        if (if_node->getFalseStmt()) {
            if ((rightContinue = codegen_stmts(ctx, if_node->getFalseStmt())))
                ctx->getBuilder()->CreateBr(doneBlock);
        }
        else
            ctx->getBuilder()->CreateBr(doneBlock);

        // 'doneBlock' remains on the stack
        if (leftContinue || rightContinue)
            ctx->pushBlock(doneBlock);
        // We ended up not using this block, so delete it
        else
            doneBlock->eraseFromParent();

        // If both branches return, the caller should stop, or we'll end up
        // creating a block with no predecessors
        return leftContinue || rightContinue;
    }
    // Expression statement
    else if (std::shared_ptr<ASTExprStmt> exp_stmt = std::dynamic_pointer_cast<ASTExprStmt>(head))
        codegen_exp(ctx, exp_stmt->getExp());
    else
        throw new ASTMalformedException();

    return true;
}

void codegen_tops(std::shared_ptr<ModuleInfo> module, std::shared_ptr<ASTTopSeqNode> nodes, bool emitDevice, std::ostream & out) {
    CodegenCtx ctx(emitDevice, module);

    std::shared_ptr<ASTTopSeqNode> node = nodes;

    while (node != NULL) {
        std::shared_ptr<ASTTopNode> top_node = node->getHead();

        // Create LLVM functions for each function
        if (std::shared_ptr<ASTFunDefnTop> funDefn = std::dynamic_pointer_cast<ASTFunDefnTop>(top_node)) {
            std::shared_ptr<FunctionInfo> funInfo = module->getFunction(funDefn->getName());
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

void codegen_top(CodegenCtx *ctx, std::shared_ptr<ASTTopNode> node) {
    if (std::shared_ptr<ASTFunDefnTop> funDefn = std::dynamic_pointer_cast<ASTFunDefnTop>(node)) {
        std::shared_ptr<FunctionInfo> func = ctx->getModuleInfo()->getFunction(funDefn->getName());

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
