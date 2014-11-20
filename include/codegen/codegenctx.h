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
#include <statics/symboltable.h>
#include <statics/functioninfo.h>

namespace Codegen {

class CodegenCtx {
private:

    Module *module;
    BasicBlock *def_bblock, *first_bblock;
    LLVMContext & context;
    Function *cc_main;
    bool emit_device;
    IRBuilder<> *def_builder, *body_builder;
    SymbolTable<Value *> symbols;
    std::vector<BasicBlock *> blocks;
    FunctionInfo *func;

public:

    CodegenCtx(bool emit_device, FunctionInfo *func);

    void markKernel(Function *kernel);

    void emit(std::ostream & out);

    Module *getModule();

    LLVMContext & getContext();

    Function *getFunction();

    bool getEmitDevice();

    IRBuilder<> *getBuilder();

    Value *getSymbol(std::string symbol);

    BasicBlock *createBlock();

    void pushBlock(BasicBlock *block);

    void popBlock();

};

}

#endif
