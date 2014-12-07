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
#include <ast/stmt/astassignstmt.h>
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
#include <ast/stmt/astwhilestmt.h>
#include <ast/decl/asttypedecl.h>

// TODO: Alloca alignment
// TODO: Set the data layout explicitly

// Note: This file makes heavy use of ctx->getBuilder(). This is an easy way to ensure
// that the IR builder used to create an instruction always refers to the correct
// basic block. The same builder is reused until it is no longer valid, so this is not
// terribly inefficient.

namespace Codegen {

// Generates code that checks if a pointer is null, if memory safety checks
// are enabled
void create_null_check(std::shared_ptr<CodegenCtx> ctx, Value *ptr) {
    CCArgs *args = getOptions();

    if (args->mem_safe) {
        std::vector<Value *> args;
        args.push_back(ctx->getBuilder()->CreatePointerCast(
            ptr, PointerType::getUnqual(Type::getInt8Ty(ctx->getContext()))));

        ctx->getBuilder()->CreateCall(ctx->getDerefCheck(), args);
    }
}

// Generates code that checks if a pointer to an array if null, and if an index
// into that array is >= 0 and < length of the array, if memory safety checks
// are enabled
void create_bounds_check(std::shared_ptr<CodegenCtx> ctx, Value *arr, Value *sub) {
    CCArgs *args = getOptions();

    if (args->mem_safe) {
        std::vector<Value *> args;
        args.push_back(ctx->getBuilder()->CreatePointerCast(
            arr, PointerType::getUnqual(Type::getInt8Ty(ctx->getContext()))));
        args.push_back(sub);

        ctx->getBuilder()->CreateCall(ctx->getArrBoundsCheck(), args);
    }
}

Value *codegen_lvalue(
    std::shared_ptr<CodegenCtx> ctx,
    std::shared_ptr<ASTExpNode> node,
    bool *require_null_check)
{
    *require_null_check = false;

    // Identifier reference
    if (std::shared_ptr<ASTIdentifierExp> id_exp = std::dynamic_pointer_cast<ASTIdentifierExp>(node)) {
        Value *id_ptr = ctx->getOrCreateSymbol(id_exp->getId());

        // Identifiers are stack allocated until mem2reg. We know that their
        // addresses are safe. No need for a null check.

        return id_ptr;
    }
    // Array subscript
    else if (std::shared_ptr<ASTIndexExp> idx_exp = std::dynamic_pointer_cast<ASTIndexExp>(node)) {
        Value *lhs = codegen_exp(ctx, idx_exp->getLValue());
        Value *sub = codegen_exp(ctx, idx_exp->getSubscript());

        create_bounds_check(ctx, lhs, sub);

        // We know that if the array is not null, and the index is in bounds,
        // we won't be returning a null pointer.

        return ctx->getBuilder()->CreateGEP(lhs, sub);
    }
    // Record access
    else if (std::shared_ptr<ASTRecordAccessExp> rcd_exp = std::dynamic_pointer_cast<ASTRecordAccessExp>(node)) {
        bool check;
        Value *lhs = codegen_lvalue(ctx, rcd_exp->getLValue(), &check);

        if (check)
            create_null_check(ctx, lhs);

        std::shared_ptr<ASTRecordType> recSig = std::static_pointer_cast<ASTRecordType>(
            rcd_exp->getLValue()->getType());
        int field_idx = recSig->getFieldIndex(rcd_exp->getId());

        // We know that if the struct is not null, a member of that struct will
        // not be at a null address.

        return ctx->getBuilder()->CreateConstGEP2_32(lhs, 0, field_idx);
    }
    // Pointer dereference
    else if (std::shared_ptr<ASTDerefExp> ptr_exp = std::dynamic_pointer_cast<ASTDerefExp>(node)) {
        Value *ptr_val = codegen_exp(ctx, ptr_exp->getExp());

        // We're essentially returning the pointer directly, so it might be null
        *require_null_check = true;

        // TODO: Don't really need this?
        return ctx->getBuilder()->CreateGEP(ptr_val, ConstantInt::get(convertType(ASTIntegerType::get(), ctx.get()), 0));
    }
    else
        throw new ASTMalformedException();

    return nullptr;
}

Value *codegen_binop(
    std::shared_ptr<CodegenCtx> ctx,
    enum ASTBinopExp::op op,
    std::shared_ptr<ASTTypeNode> type,
    Value *v1,
    Value *v2)
{
    CCArgs *args = getOptions();

    bool isFloat = type->equal(ASTFloatType::get());

    switch (op) {
    case ASTBinopExp::ADD:  return ctx->getBuilder()->CreateBinOp(isFloat ? Instruction::FAdd : Instruction::Add, v1, v2);
    case ASTBinopExp::SUB:  return ctx->getBuilder()->CreateBinOp(isFloat ? Instruction::FSub : Instruction::Sub, v1, v2);
    case ASTBinopExp::MUL:  return ctx->getBuilder()->CreateBinOp(isFloat ? Instruction::FMul : Instruction::Mul, v1, v2);
    case ASTBinopExp::DIV:
    case ASTBinopExp::MOD:
        // Insert division checks
        if (args->opr_safe) {
            std::vector<Value *> args;
            args.push_back(v1);
            args.push_back(v2);

            ctx->getBuilder()->CreateCall(ctx->getDivCheck(), args);
        }

        if (op == ASTBinopExp::DIV)
            return ctx->getBuilder()->CreateBinOp(isFloat ? Instruction::FDiv : Instruction::SDiv, v1, v2);
        else
            return ctx->getBuilder()->CreateBinOp(isFloat ? Instruction::FRem : Instruction::SRem, v1, v2);
    case ASTBinopExp::SHL:
    case ASTBinopExp::SHR:
        // Insert bounds checks
        if (args->opr_safe) {
            std::vector<Value *> args;
            args.push_back(v2);

            ctx->getBuilder()->CreateCall(ctx->getShiftCheck(), args);
        }

        if (op == ASTBinopExp::SHL)
            return ctx->getBuilder()->CreateBinOp(Instruction::Shl, v1, v2);
        else
            return ctx->getBuilder()->CreateBinOp(Instruction::AShr, v1, v2);
    case ASTBinopExp::AND:  return ctx->getBuilder()->CreateBinOp(Instruction::And, v1, v2);
    case ASTBinopExp::OR:   return ctx->getBuilder()->CreateBinOp(Instruction::Or, v1, v2);
    case ASTBinopExp::BAND: return ctx->getBuilder()->CreateBinOp(Instruction::And, v1, v2);
    case ASTBinopExp::BOR:  return ctx->getBuilder()->CreateBinOp(Instruction::Or, v1, v2);
    case ASTBinopExp::BXOR: return ctx->getBuilder()->CreateBinOp(Instruction::Xor, v1, v2);
    case ASTBinopExp::EQ:   return (isFloat ? ctx->getBuilder()->CreateFCmpOEQ(v1, v2) : ctx->getBuilder()->CreateICmpEQ(v1, v2));
    case ASTBinopExp::NEQ:  return (isFloat ? ctx->getBuilder()->CreateFCmpONE(v1, v2) : ctx->getBuilder()->CreateICmpNE(v1, v2));
    case ASTBinopExp::LEQ:  return (isFloat ? ctx->getBuilder()->CreateFCmpOLE(v1, v2) : ctx->getBuilder()->CreateICmpSLE(v1, v2));
    case ASTBinopExp::GEQ:  return (isFloat ? ctx->getBuilder()->CreateFCmpOGE(v1, v2) : ctx->getBuilder()->CreateICmpSGE(v1, v2));
    case ASTBinopExp::LT:   return (isFloat ? ctx->getBuilder()->CreateFCmpOLT(v1, v2) : ctx->getBuilder()->CreateICmpSLT(v1, v2));
    case ASTBinopExp::GT:   return (isFloat ? ctx->getBuilder()->CreateFCmpOGT(v1, v2) : ctx->getBuilder()->CreateICmpSGT(v1, v2));
    case ASTBinopExp::NONE: throw ASTMalformedException(); return nullptr;
    }

    throw ASTMalformedException();
    return nullptr;
}

Value *codegen_exp(std::shared_ptr<CodegenCtx> ctx, std::shared_ptr<ASTExpNode> node) {
    // Integer constant
    if (std::shared_ptr<ASTIntegerExp> int_exp = std::dynamic_pointer_cast<ASTIntegerExp>(node))
        return ConstantInt::get(convertType(ASTIntegerType::get(), ctx.get()), int_exp->getValue());
    // Boolean constant
    else if (std::shared_ptr<ASTBooleanExp> bool_exp = std::dynamic_pointer_cast<ASTBooleanExp>(node))
        return ConstantInt::get(convertType(ASTBooleanType::get(), ctx.get()), (int)bool_exp->getValue());
    // Float constant
    else if (std::shared_ptr<ASTFloatExp> float_exp = std::dynamic_pointer_cast<ASTFloatExp>(node))
        return ConstantFP::get(convertType(ASTFloatType::get(), ctx.get()), float_exp->getValue());
    else if (std::shared_ptr<ASTNullExp> null_exp = std::dynamic_pointer_cast<ASTNullExp>(node)) {
        Type *cvt_type = convertType(null_exp->getType(), ctx.get());
        return ConstantPointerNull::get(static_cast<PointerType *>(cvt_type));
    }
    // Unary operator
    else if (std::shared_ptr<ASTUnopExp> unop_exp = std::dynamic_pointer_cast<ASTUnopExp>(node)) {
        Value *v = codegen_exp(ctx, unop_exp->getExp());

        switch(unop_exp->getOp()) {
        case ASTUnopExp::NOT:  return ctx->getBuilder()->CreateBinOp(Instruction::Xor, v,
                                   ConstantInt::get(Type::getInt1Ty(ctx->getContext()), 1));
        case ASTUnopExp::BNOT: return ctx->getBuilder()->CreateNot(v);
        case ASTUnopExp::NEG:  return ctx->getBuilder()->CreateNeg(v); // TODO investigate x86
        }
    }
    // Binary operator
    else if (std::shared_ptr<ASTBinopExp> binop_exp = std::dynamic_pointer_cast<ASTBinopExp>(node)) {
        Value *v1 = codegen_exp(ctx, binop_exp->getE1());
        Value *v2 = codegen_exp(ctx, binop_exp->getE2());

        return codegen_binop(ctx, binop_exp->getOp(), binop_exp->getE1()->getType(), v1, v2);
    }
    // Ternary operator
    else if (std::shared_ptr<ASTTernopExp> tern_exp = std::dynamic_pointer_cast<ASTTernopExp>(node)) {
        Value *cond = codegen_exp(ctx, tern_exp->getCond());

        std::shared_ptr<ASTTypeNode> expType = tern_exp->getType();
        assert(expType != nullptr);
        Value *result = ctx->createTemp(convertType(expType, ctx.get()));

        BasicBlock *trueBlock = ctx->createBlock();
        BasicBlock *falseBlock = ctx->createBlock();
        BasicBlock *doneBlock = ctx->createBlock();

        // Generate conditional jump
        ctx->getBuilder()->CreateCondBr(cond, trueBlock, falseBlock);

        // Generate 'true' branch
        ctx->pushBlock(trueBlock);
        Value *trueVal = codegen_exp(ctx, tern_exp->getTrueExp());
        ctx->getBuilder()->CreateStore(trueVal, result);
        ctx->getBuilder()->CreateBr(doneBlock);

        // Generate 'false' branch
        ctx->pushBlock(falseBlock);
        Value *falseVal = codegen_exp(ctx, tern_exp->getFalseExp());
        ctx->getBuilder()->CreateStore(falseVal, result);
        ctx->getBuilder()->CreateBr(doneBlock);

        ctx->pushBlock(doneBlock);

        return ctx->getBuilder()->CreateLoad(result);
    }
    // Function call
    else if (std::shared_ptr<ASTCallExp> call_exp = std::dynamic_pointer_cast<ASTCallExp>(node)) {
        std::vector<Value *> args;

        Value *ret_val = nullptr;

        std::shared_ptr<FunctionInfo> funcInfo = ctx->getModuleInfo()->getFunction(call_exp->getId());
        std::shared_ptr<ASTFunType> sig = funcInfo->getSignature();
        bool isVoid = sig->getReturnType()->equal(ASTVoidType::get());

        // In device mode, add a pointer to a new temp to get the return value
        if (ctx->getEmitDevice() && (funcInfo->getUsage() & FunctionInfo::Global) && !isVoid) {
            // TODO use an address of instead maybe
            ret_val = ctx->createTemp(convertType(sig->getReturnType(), ctx.get()));
            args.push_back(ret_val);
        }

        std::shared_ptr<ASTExpSeqNode> exp_args = call_exp->getArgs();

        // Codegen each argument
        while (exp_args != nullptr) {
            std::shared_ptr<ASTExpNode> arg = exp_args->getHead();
            args.push_back(codegen_exp(ctx, arg));
            exp_args = exp_args->getTail();
        }

        Value *call = ctx->getBuilder()->CreateCall(ctx->getFunction(call_exp->getId()), args);

        if (!ctx->getEmitDevice() || !(funcInfo->getUsage() & FunctionInfo::Global))
            ret_val = call;
        else if (!isVoid)
            ret_val = ctx->getBuilder()->CreateLoad(ret_val);

        // If the function has a void return type, we'll return nullptr, but this should have
        // already been handled by the type checker: can't assign a void expression to any
        // type of lvalue.
        return ret_val;
    }
    // Array allocation
    else if (std::shared_ptr<ASTAllocArrayExp> alloc_exp = std::dynamic_pointer_cast<ASTAllocArrayExp>(node)) {
        // Element size constant
        Value *elemSize = ConstantInt::get(convertType(ASTIntegerType::get(), ctx.get()),
            ctx->getAlignedSize((alloc_exp->getElemType())));

        // Array length
        Value *length = codegen_exp(ctx, alloc_exp->getLength());

        // Call into runtime allocator
        std::vector<Value *> args;
        args.push_back(elemSize);
        args.push_back(length);

        Value *buff = ctx->getBuilder()->CreateCall(ctx->getAllocArray(), args);

        // Cast the result to the right type
        return ctx->getBuilder()->CreatePointerCast(buff,
            PointerType::getUnqual(convertType(alloc_exp->getElemType(), ctx.get())));
    }
    // Heap allocation
    else if (std::shared_ptr<ASTAllocExp> alloc_exp = std::dynamic_pointer_cast<ASTAllocExp>(node)) {
        // Element size constant
        Value *elemSize = ConstantInt::get(convertType(ASTIntegerType::get(), ctx.get()),
            ctx->getAlignedSize((alloc_exp->getElemType())));

        // Call into runtime allocator
        std::vector<Value *> args;
        args.push_back(elemSize);

        Value *buff = ctx->getBuilder()->CreateCall(ctx->getAlloc(), args);

        // Cast the result to the right type
        return ctx->getBuilder()->CreatePointerCast(buff,
            PointerType::getUnqual(convertType(alloc_exp->getElemType(), ctx.get())));
    }
    // Otherwise, it's an lvalue. Get the address and dereference it.
    else {
        bool check;
        Value *lval_ptr = codegen_lvalue(ctx, node, &check);

        if (check)
            create_null_check(ctx, lval_ptr);

        return ctx->getBuilder()->CreateLoad(lval_ptr);
    }

    throw ASTMalformedException();
    return nullptr;
}

bool codegen_stmts(std::shared_ptr<CodegenCtx> ctx, std::shared_ptr<ASTStmtSeqNode> seq_node) {
    while (seq_node != nullptr) {
        if (!codegen_stmt(ctx, seq_node->getHead()))
            return false;
        seq_node = seq_node->getTail();
    }

    return true;
}

Value *genDeviceAlloc(std::shared_ptr<CodegenCtx> ctx, int size) {
    std::vector<Value *> args;
    args.push_back(ConstantInt::get(Type::getInt32Ty(ctx->getContext()), size));

    return ctx->getBuilder()->CreateCall(ctx->getAllocDevice(), args);
}

Value *genCopyHostToDevice(std::shared_ptr<CodegenCtx> ctx, Value *dst, Value *src, int size) {
    std::vector<Value *> args;
    args.push_back(dst);
    // Cast to char *
    args.push_back(ctx->getBuilder()->CreatePointerCast(
        src,
        PointerType::getUnqual(Type::getInt8Ty(ctx->getContext()))));
    args.push_back(ConstantInt::get(Type::getInt32Ty(ctx->getContext()), size));

    return ctx->getBuilder()->CreateCall(ctx->getCopyHostToDevice(), args);
}

void codegen_cudacall(
    std::shared_ptr<CodegenCtx> ctx,
    Value *min,
    Value *max,
    std::shared_ptr<ASTCallExp> call)
{
    std::shared_ptr<FunctionInfo> fun = ctx->getModuleInfo()->getFunction(call->getId());
    std::shared_ptr<ASTFunType> funType = fun->getSignature();

    std::vector<Value *> argPtrs;

    Value *arg_buff = ctx->getBuilder()->CreateAlloca(
        PointerType::getUnqual(Type::getInt8Ty(ctx->getContext())),
        ConstantInt::get(Type::getInt32Ty(ctx->getContext()), funType->getNumArgs()));

    std::shared_ptr<ASTArgSeqNode> args_sig = funType->getArgs();
    std::shared_ptr<ASTExpSeqNode> args_val = call->getArgs();
    int argIdx = 0;

    // TODO: return value

    // Generate code for arguments and copy them to GPU memory
    while (args_sig != nullptr) {
        std::shared_ptr<ASTArgNode> arg_sig = args_sig->getHead();
        std::shared_ptr<ASTExpNode> arg_exp = args_val->getHead();

        // Generate code for the argument, allocate room for it, and copy it
        // The 'source' is a pointer to a stack location, so we don't need to
        // take its address to copy it.
        unsigned long arg_size = ctx->getSize(arg_sig->getType());
        Value *arg_src = codegen_exp(ctx, arg_exp);
        Value *arg_dst = genDeviceAlloc(ctx, arg_size);
        genCopyHostToDevice(ctx, arg_dst, arg_src, arg_size);
        //argPtrs.push_back(arg_dst);

        // Store pointer into argument buffer
        Value *argIdxVal = ConstantInt::get(Type::getInt32Ty(ctx->getContext()), argIdx++);
        Value *argDstPtr = ctx->getBuilder()->CreateGEP(arg_buff, argIdxVal);
        ctx->getBuilder()->CreateStore(arg_dst, argDstPtr);

        args_sig = args_sig->getTail();
        args_val = args_val->getTail();
    }

    // Invoke the kernel
    std::vector<Value *> invokeArgs;
    // TODO: names get duplicated
    invokeArgs.push_back(ctx->getBuilder()->CreateGlobalStringPtr(call->getId()));
    invokeArgs.push_back(arg_buff);

    ctx->getBuilder()->CreateCall(ctx->getInvokeKernel(), invokeArgs);

    // TODO: copy back to host, free
}

bool codegen_stmt(std::shared_ptr<CodegenCtx> ctx, std::shared_ptr<ASTStmtNode> head) {
    // Return instruction
    if (std::shared_ptr<ASTReturnStmt> ret_node = std::dynamic_pointer_cast<ASTReturnStmt>(head)) {
        std::shared_ptr<ASTExpNode> ret_exp = ret_node->getExp();

        // Void expressions don't need to return a value
        if (ret_exp) {
            Value *ret_val = codegen_exp(ctx, ret_exp);

            // In device mode, have to move into return value argument because kernels
            // must return void
            if (ctx->getEmitDevice() && (ctx->getCurrentFunctionInfo()->getUsage() & FunctionInfo::Global)) {
                Value *out_arg = ctx->getCurrentFunction()->arg_begin();

                ctx->getBuilder()->CreateStore(ret_val, out_arg);
                ctx->getBuilder()->CreateRet(nullptr);
            }
            else
                ctx->getBuilder()->CreateRet(ret_val);
        }
        else
            ctx->getBuilder()->CreateRet(nullptr);

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
            ctx->getBuilder()->CreateStore(exp_val, mem);
        }
    }
    // Assignment to an lvalue. Get the address and write to it. Note that this works for
    // local variables as well because they are stack allocated until conversion to SSA.
    else if (std::shared_ptr<ASTAssignStmt> decl_stmt = std::dynamic_pointer_cast<ASTAssignStmt>(head)) {
        bool check;
        Value *lval = codegen_lvalue(ctx, decl_stmt->getLValue(), &check);

        if (decl_stmt->getOp() == ASTBinopExp::NONE) {
            Value *rhs = codegen_exp(ctx, decl_stmt->getExp());

            if (check)
                create_null_check(ctx, lval);

            ctx->getBuilder()->CreateStore(rhs, lval);
        }
        // Compound assignment like +=, etc.
        else {
            Value *rhs = codegen_exp(ctx, decl_stmt->getExp());

            if (check)
                create_null_check(ctx, lval);

            Value *loadLVal = ctx->getBuilder()->CreateLoad(lval);

            Value *newVal = codegen_binop(ctx, decl_stmt->getOp(),
                decl_stmt->getExp()->getType(), loadLVal, rhs);
            ctx->getBuilder()->CreateStore(newVal, lval);
        }
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
    // Assert statement
    else if (std::shared_ptr<ASTAssertStmt> assert_node = std::dynamic_pointer_cast<ASTAssertStmt>(head)) {
        Value *cond = codegen_exp(ctx, assert_node->getCond());

        std::vector<Value *> args;
        args.push_back(cond);

        ctx->getBuilder()->CreateCall(ctx->getAssert(), args);
    }
    // While statement
    else if (std::shared_ptr<ASTWhileStmt> while_node = std::dynamic_pointer_cast<ASTWhileStmt>(head)) {
        Value *cond = codegen_exp(ctx, while_node->getCond());

        BasicBlock *bodyBlock = ctx->createBlock();
        BasicBlock *doneBlock = ctx->createBlock();

        // Generate conditional jump
        ctx->getBuilder()->CreateCondBr(cond, bodyBlock, doneBlock);

        // Generate while's 'body' branch
        ctx->pushBlock(bodyBlock);

        // Whether the body of the while loop returns
        bool bodyContinue;

        // Only need to insert looping conditional jump if body didn't return
        if ((bodyContinue = codegen_stmts(ctx, while_node->getBody()))) {
            Value *body_cond = codegen_exp(ctx, while_node->getCond());
            ctx->getBuilder()->CreateCondBr(body_cond, bodyBlock, doneBlock);
        }
        // 'doneBlock' remains on the stack
        ctx->pushBlock(doneBlock);

        return true;
    }
    // For statement
    else if (std::shared_ptr<ASTForStmt> for_node = std::dynamic_pointer_cast<ASTForStmt>(head)) {
        // Generate the initialization code
        if (for_node->getInit())
            codegen_stmt(ctx, for_node->getInit());

        // Generate the condition
        Value *cond = codegen_exp(ctx, for_node->getCond());

        BasicBlock *bodyBlock = ctx->createBlock();
        BasicBlock *doneBlock = ctx->createBlock();

        // Generate conditional jump
        ctx->getBuilder()->CreateCondBr(cond, bodyBlock, doneBlock);

        // Generate while's 'body' branch
        ctx->pushBlock(bodyBlock);

        // Whether the body of the while loop returns
        bool bodyContinue = codegen_stmts(ctx, for_node->getBody());

        // Only need to insert looping conditional jump if body didn't return
        if (bodyContinue) {
            if (for_node->getIter())
                codegen_stmt(ctx, for_node->getIter());

            Value *body_cond = codegen_exp(ctx, for_node->getCond());
            ctx->getBuilder()->CreateCondBr(body_cond, bodyBlock, doneBlock);
        }

        // 'doneBlock' remains on the stack
        ctx->pushBlock(doneBlock);

        return true;
    }
    // Expression statement
    else if (std::shared_ptr<ASTExprStmt> exp_stmt = std::dynamic_pointer_cast<ASTExprStmt>(head))
        codegen_exp(ctx, exp_stmt->getExp());
    else
        throw new ASTMalformedException();

    return true;
}

bool shouldGenerateCodeForFunction(std::shared_ptr<CodegenCtx> ctx, std::shared_ptr<ASTFunDecl> decl) {
    bool declare = true;

    std::shared_ptr<FunctionInfo> func = ctx->getModuleInfo()->getFunction(decl->getName());

    // Skip declarations that don't define bodies
    // TODO: ignore multiple declarations of external functions
    if (!decl->isDefn())
        declare = false;

    // Skip global and device functions in host mode
    if (!ctx->getEmitDevice() && !(func->getUsage() & FunctionInfo::Host))
        declare = false;

    // Skip host functions in device mode
    if (ctx->getEmitDevice() && !(func->getUsage() & (FunctionInfo::Global | FunctionInfo::Device)))
        declare = false;

    return declare;
}

void codegen_tops(std::shared_ptr<ModuleInfo> module, std::shared_ptr<ASTDeclSeqNode> nodes, bool emitDevice, std::ostream & out) {
    std::shared_ptr<CodegenCtx> ctx = std::make_shared<CodegenCtx>(emitDevice, module);

    std::shared_ptr<ASTDeclSeqNode> node = nodes;

    while (node != nullptr) {
        std::shared_ptr<ASTDeclNode> top_node = node->getHead();

        // Create LLVM functions for each function
        if (std::shared_ptr<ASTFunDecl> funDefn = std::dynamic_pointer_cast<ASTFunDecl>(top_node)) {
            if (shouldGenerateCodeForFunction(ctx, funDefn) || funDefn->getLinkage() == ASTDeclNode::External) {
                std::shared_ptr<FunctionInfo> funInfo = module->getFunction(funDefn->getName());
                ctx->createFunction(funInfo);
            }
        }

        node = node->getTail();
    }

    node = nodes;

    while (node != nullptr) {
        codegen_top(ctx, node->getHead());
        node = node->getTail();
    }

    ctx->emit(out);
}

void codegen_dim_vars(std::shared_ptr<CodegenCtx> ctx, std::shared_ptr<ASTFunDecl> funDefn) {
    std::shared_ptr<FunctionInfo> func = ctx->getModuleInfo()->getFunction(funDefn->getName());

    std::shared_ptr<ASTArgSeqNode> rangeArgs = funDefn->getSignature()->getDimArgs();

    int argIdx = 0;

    std::vector<Type *> argTypes;
    FunctionType *intrinType = FunctionType::get(Type::getInt32Ty(ctx->getContext()), argTypes, false);

    while (rangeArgs != nullptr) {
        std::shared_ptr<ASTArgNode> arg = rangeArgs->getHead();
        Value *local = ctx->getOrCreateSymbol(arg->getName());

        Constant *getTID = nullptr;
        Constant *getNTID = nullptr;
        Constant *getCTAID = nullptr;

        switch(argIdx++) {
        case 0:
            getTID = ctx->getModule()->getOrInsertFunction("llvm.nvvm.read.ptx.sreg.tid.x", intrinType);
            getNTID = ctx->getModule()->getOrInsertFunction("llvm.nvvm.read.ptx.sreg.ntid.x", intrinType);
            getCTAID = ctx->getModule()->getOrInsertFunction("llvm.nvvm.read.ptx.sreg.ctaid.x", intrinType);
            break;
        case 1:
            getTID = ctx->getModule()->getOrInsertFunction("llvm.nvvm.read.ptx.sreg.tid.y", intrinType);
            getNTID = ctx->getModule()->getOrInsertFunction("llvm.nvvm.read.ptx.sreg.ntid.y", intrinType);
            getCTAID = ctx->getModule()->getOrInsertFunction("llvm.nvvm.read.ptx.sreg.ctaid.y", intrinType);
            break;
        case 2:
            getTID = ctx->getModule()->getOrInsertFunction("llvm.nvvm.read.ptx.sreg.tid.z", intrinType);
            getNTID = ctx->getModule()->getOrInsertFunction("llvm.nvvm.read.ptx.sreg.ntid.z", intrinType);
            getCTAID = ctx->getModule()->getOrInsertFunction("llvm.nvvm.read.ptx.sreg.ctaid.z", intrinType);
            break;
        default:
            throw ASTMalformedException();
        }

        Value *tid = ctx->getBuilder()->CreateCall(getTID);
        Value *ntid = ctx->getBuilder()->CreateCall(getNTID);
        Value *ctaid = ctx->getBuilder()->CreateCall(getCTAID);

        Value *threadId =
            ctx->getBuilder()->CreateBinOp(Instruction::Add,
                tid,
                ctx->getBuilder()->CreateBinOp(Instruction::Mul, ctaid, ntid));

        ctx->getBuilder()->CreateStore(threadId, local);

        rangeArgs = rangeArgs->getTail();
    }
}

void codegen_top(std::shared_ptr<CodegenCtx> ctx, std::shared_ptr<ASTDeclNode> node) {
    if (std::shared_ptr<ASTFunDecl> funDefn = std::dynamic_pointer_cast<ASTFunDecl>(node)) {
        if (!shouldGenerateCodeForFunction(ctx, funDefn))
            return;

        if (funDefn->getLinkage() == ASTDeclNode::External)
            return;

        std::shared_ptr<FunctionInfo> func = ctx->getModuleInfo()->getFunction(funDefn->getName());

        ctx->startFunction(func->getName());

        if (func->getUsage() & FunctionInfo::Global)
            codegen_dim_vars(ctx, funDefn);

        codegen_stmts(ctx, funDefn->getBody());

        if (func->getUsage() & FunctionInfo::Global)
            ctx->markKernel(ctx->getFunction(func->getName()));

        ctx->finishFunction();
    }
    else if (std::shared_ptr<ASTRecordDecl> rcdDecl = std::dynamic_pointer_cast<ASTRecordDecl>(node)) {
        if (rcdDecl->isDefn())
            ctx->createRecord(rcdDecl->getSignature());
    }
}

}
