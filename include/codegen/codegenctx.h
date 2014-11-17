/**
 * @file codegenctx.h
 *
 * @brief Code generation contet
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __CODEGENCTX_H
#define __CODEGENCTX_H

#include <defs.h>

#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/PassManager.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/IRPrintingPasses.h>

using namespace llvm;

class CodegenCtx {
private:

    Module *module;
    BasicBlock *bblock;
    LLVMContext & context;

public:

    CodegenCtx()
        : context(getGlobalContext())
    {
        module = new Module("", context);

        std::vector<Type *> argTypes;
        FunctionType *ftype = FunctionType::get(Type::getInt32Ty(context), argTypes, false);

        Function *cc_main = Function::Create(ftype, GlobalValue::ExternalLinkage, "_cc_main", module);

        bblock = BasicBlock::Create(context, "entry", cc_main, 0);
    }

    void finish(Value *retval) {
        ReturnInst::Create(context, retval, bblock);

        PassManager pm;
        pm.add(createPrintModulePass(outs()));
        pm.run(*module);
    }

    Module *getModule() {
        return module;
    }

    LLVMContext & getContext() {
        return context;
    }

    BasicBlock *getBBlock() {
        return bblock;
    }

};

#endif
