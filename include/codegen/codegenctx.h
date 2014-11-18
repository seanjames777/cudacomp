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
    IRBuilder<> *builder;

public:

    CodegenCtx(bool emit_device);

    void markKernel(Function *kernel);

    void emit(char *out_file);

    Module *getModule();

    LLVMContext & getContext();

    BasicBlock *getBBlock();

    Function *getFunction();

    bool getEmitDevice();

    IRBuilder<> *getBuilder();

};

#endif
