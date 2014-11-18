/**
 * @file codegenctx.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <codegen/codegenctx.h>

CodegenCtx::CodegenCtx(bool emit_device)
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

void CodegenCtx::markKernel(Function *kernel) {
    std::vector<Value *> meta;
    meta.push_back(cc_main);
    meta.push_back(MDString::get(context, "kernel"));
    meta.push_back(ConstantInt::get(Type::getInt32Ty(context), 1));
    MDNode *node = MDNode::get(context, meta);

    NamedMDNode *cat = module->getOrInsertNamedMetadata("nvvm.annotations");
    cat->addOperand(node);
}

void CodegenCtx::emit(Value *retval, char *out_file) {
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

Module *CodegenCtx::getModule() {
    return module;
}

LLVMContext & CodegenCtx::getContext() {
    return context;
}

BasicBlock *CodegenCtx::getBBlock() {
    return bblock;
}
