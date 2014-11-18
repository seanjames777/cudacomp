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
#include <statics/typectx.h>
#include <statics/typecheck.h>

class CodegenCtx {
private:

    Module *module;
    BasicBlock *def_bblock, *body_bblock;
    LLVMContext & context;
    Function *cc_main;
    bool emit_device;
    IRBuilder<> *def_builder, *body_builder;
    TypeCtx *types;
    std::unordered_map<std::string, Value *> symbols;

public:

    CodegenCtx(bool emit_device, TypeCtx *types);

    void markKernel(Function *kernel);

    void emit(char *out_file);

    Module *getModule();

    LLVMContext & getContext();

    BasicBlock *getBBlock();

    Function *getFunction();

    bool getEmitDevice();

    IRBuilder<> *getBuilder();

    Type *getType(ASTExpNode *exp);

    Value *getSymbol(std::string symbol);

};

#endif
