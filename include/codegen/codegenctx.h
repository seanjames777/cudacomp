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
#include <llvm/ADT/Triple.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/IR/IRBuilder.h>

using namespace llvm;

class CodegenCtx {
private:

    Module *module;
    BasicBlock *bblock;
    LLVMContext & context;
    Function *cc_main;
    bool emit_device;

public:

    CodegenCtx(bool emit_device)
        : context(getGlobalContext()),
          emit_device(emit_device)
    {
        module = new Module("", context);

        std::vector<Type *> argTypes;
        argTypes.push_back(Type::getInt32PtrTy(context));
        FunctionType *ftype = FunctionType::get(Type::getVoidTy(context), argTypes, false);

        cc_main = Function::Create(ftype, GlobalValue::ExternalLinkage, "_cc_main", module);

        bblock = BasicBlock::Create(context, "entry", cc_main, 0);
    }

    void markKernel(Function *kernel) {
        std::vector<Value *> meta;
        meta.push_back(cc_main);
        meta.push_back(MDString::get(context, "kernel"));
        meta.push_back(ConstantInt::get(Type::getInt32Ty(context), 1));
        MDNode *node = MDNode::get(context, meta);

        NamedMDNode *cat = module->getOrInsertNamedMetadata("nvvm.annotations");
        cat->addOperand(node);
    }

    void emit(Value *retval, char *out_file) {
        Value *out_arg = cc_main->arg_begin();

        IRBuilder<> builder(bblock);

        builder.CreateStore(retval, out_arg);
        builder.CreateRet(NULL);

        if (emit_device) {
            markKernel(cc_main);
            module->setTargetTriple("nvptx64-nvidia-cuda");
        }
        else {
            module->setTargetTriple("x86_64-apple-macosx10.10.0");
        }

        PassManager pm;

        if (!out_file) {
            pm.add(createPrintModulePass(outs()));
            pm.run(*module);
        }
        else {
            std::ofstream out(out_file, std::ios::out);

            if (!out) {
                // TODO explode
                return;
            }

            raw_os_ostream outs(out);

            pm.add(createPrintModulePass(outs));

            pm.run(*module);

            out.close();
        }
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
