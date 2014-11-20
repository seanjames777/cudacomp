/**
 * @file codegenctx.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <codegen/codegenctx.h>
#include <codegen/converttype.h>

namespace Codegen {

CodegenCtx::CodegenCtx(bool emit_device, ModuleInfo *modInfo)
    : module(NULL),
      context(getGlobalContext()),
      emit_device(emit_device),
      modInfo(modInfo),
      def_bblock(NULL),
      first_bblock(NULL),
      def_builder(NULL),
      function(NULL),
      funcInfo(NULL),
      body_builder(NULL)
{
    module = new Module("", context);
}

ModuleInfo *CodegenCtx::getModuleInfo() {
    return modInfo;
}

Module *CodegenCtx::getModule() {
    return module;
}

bool CodegenCtx::getEmitDevice() {
    return emit_device;
}

LLVMContext & CodegenCtx::getContext() {
    return context;
}

void CodegenCtx::emit(std::ostream & out) {
    if (emit_device)
        module->setTargetTriple("nvptx64-nvidia-cuda");
    else
        module->setTargetTriple("x86_64-apple-macosx10.10.0");

    PassManager pm;

    raw_os_ostream outs(out);

    // Replace memory operands with registers where possible
    pm.add(createPromoteMemoryToRegisterPass());

    // Perform some simple optimizations just to clean up the output IR
    // TODO

    // Finally, print the result to the output stream
    pm.add(createPrintModulePass(outs));

    pm.run(*module);
}

Function *CodegenCtx::getFunction(std::string id) {
    return functions.get(id);
}

Function *CodegenCtx::getCurrentFunction() {
    return function;
}

FunctionInfo *CodegenCtx::getCurrentFunctionInfo() {
    return funcInfo;
}

Function *CodegenCtx::createFunction(FunctionInfo *funcInfo) {
    std::vector<Type *> argTypes;
    FunctionType *ftype;

    ASTFunType *sig = funcInfo->getSignature();
    ASTArgSeqNode *args = sig->getArgs();
    Type *returnType = convertType(sig->getReturnType());

    if (emit_device)
        argTypes.push_back(PointerType::getUnqual(returnType));

    // Add arguments to LLVM function type
    while (args != NULL) {
        ASTArg *arg = args->getHead();
        argTypes.push_back(convertType(arg->getType()));
        args = args->getTail();
    }

    if (emit_device)
        ftype = FunctionType::get(Type::getVoidTy(context), argTypes, false);
    else
        ftype = FunctionType::get(returnType, argTypes, false);

    Function *function = Function::Create(ftype, GlobalValue::ExternalLinkage, funcInfo->getName(), module);
    functions.set(funcInfo->getName(), function);

    return function;
}

void CodegenCtx::startFunction(std::string id) {
    this->function = getFunction(id);
    symbols.clear();
    blocks.clear();

    def_bblock = BasicBlock::Create(context, "defs", function, NULL);
    def_builder = new IRBuilder<>(def_bblock);

    first_bblock = createBlock();
    pushBlock(first_bblock);

    funcInfo = modInfo->getFunction(id);
    ASTFunType *sig = funcInfo->getSignature();

    ASTArgSeqNode *args = sig->getArgs();
    auto arg_iter = function->arg_begin();

    if (emit_device)
        arg_iter++;

    // Map arguments to symbol table. Move arguments into alloca's functions
    while (args != NULL) {
        ASTArg *arg = args->getHead();

        // Create a new memory location, and copy the argument into it. mem2reg
        // should promote it back into a register, and the move should be
        // optimized out. TODO: test that this actually happens, or this is bad.
        Value *toVal = getOrCreateSymbol(arg->getName());
        symbols.set(arg->getName(), toVal);

        Value *argVal = arg_iter++;
        def_builder->CreateStore(argVal, toVal);

        args = args->getTail();
    }
}

BasicBlock *CodegenCtx::createBlock() {
    return BasicBlock::Create(context, "", function, NULL);
}

void CodegenCtx::pushBlock(BasicBlock *block) {
    blocks.push_back(block);
    this->body_builder = new IRBuilder<>(block);
}

void CodegenCtx::popBlock() {
    blocks.pop_back();
    BasicBlock *block = blocks.back();
    this->body_builder = new IRBuilder<>(block);
}

void CodegenCtx::markKernel(Function *kernel) {
    std::vector<Value *> meta;
    meta.push_back(function);
    meta.push_back(MDString::get(context, "kernel"));
    meta.push_back(ConstantInt::get(Type::getInt32Ty(context), 1));
    MDNode *node = MDNode::get(context, meta);

    NamedMDNode *cat = module->getOrInsertNamedMetadata("nvvm.annotations");
    cat->addOperand(node);
}

void CodegenCtx::finishFunction() {
    def_builder->CreateBr(first_bblock);
}

IRBuilder<> *CodegenCtx::getBuilder() {
    return body_builder;
}

Value *CodegenCtx::getOrCreateSymbol(std::string id) {
    // TODO: can name some or all of the values

    if (!symbols.hasSymbol(id)) {
        Value *instr = def_builder->CreateAlloca(convertType(funcInfo->getLocalType(id)));
        symbols.set(id, instr);
        return instr;
    }

    return symbols.get(id);
}

Value *CodegenCtx::createTemp(Type *type) {
    return def_builder->CreateAlloca(type);
}

}
