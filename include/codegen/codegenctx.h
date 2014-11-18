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

class CodegenCtx {
private:

    Module *module;
    BasicBlock *bblock;
    LLVMContext & context;
    Function *cc_main;
    bool emit_device;
    IRBuilder<> *builder;
    TypeCtx *types;

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

    TypeCtx *getTypes();

};

#endif
