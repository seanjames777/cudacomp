/**
 * @file codegenctx.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <codegen/codegenctx.h>
#include <codegen/converttype.h>
#include <ast/type/astvoidtype.h>

namespace Codegen {

CodegenCtx::CodegenCtx(bool emit_device, std::shared_ptr<ModuleInfo> modInfo)
    : module(nullptr),
      context(getGlobalContext()),
      emit_device(emit_device),
      modInfo(modInfo),
      def_bblock(nullptr),
      first_bblock(nullptr),
      def_builder(nullptr),
      function(nullptr),
      funcInfo(nullptr),
      body_builder(nullptr),
      alloc_array(nullptr)
{
    module = std::make_shared<Module>("", context);
}

Function *CodegenCtx::getAllocArray() {
    // Only declare it if it's used, to make small programs simpler
    if (!alloc_array) {
        // Construct a declaration of the runtime's alloc_array function
        std::vector<Type *> argTypes;
        argTypes.push_back(Type::getInt32Ty(context));
        argTypes.push_back(Type::getInt32Ty(context));

        FunctionType *ftype = FunctionType::get(PointerType::getUnqual(Type::getInt8Ty(context)), argTypes, false);
        alloc_array = Function::Create(ftype, GlobalValue::ExternalLinkage, "_rt_alloc_array", module.get());
    }

    return alloc_array;
}

std::shared_ptr<ModuleInfo> CodegenCtx::getModuleInfo() {
    return modInfo;
}

std::shared_ptr<Module> CodegenCtx::getModule() {
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

    // Replace memory operands with registers where possible
    pm.add(createPromoteMemoryToRegisterPass());

    // Perform some simple optimizations just to clean up the output IR
    // TODO

    // pm.add(createPrintModulePass(outs));

    pm.run(*module);

    // Finally, print the result to the output stream
    raw_os_ostream outs(out);
    outs << *module;
}

Function *CodegenCtx::getFunction(std::string id) {
    return functions.get(id);
}

Function *CodegenCtx::getCurrentFunction() {
    return function;
}

std::shared_ptr<FunctionInfo> CodegenCtx::getCurrentFunctionInfo() {
    return funcInfo;
}

Function *CodegenCtx::createFunction(std::shared_ptr<FunctionInfo> funcInfo) {
    std::vector<Type *> argTypes;
    FunctionType *ftype;

    std::shared_ptr<ASTFunType> sig = funcInfo->getSignature();
    std::shared_ptr<ASTArgSeqNode> args = sig->getArgs();
    Type *returnType = convertType(sig->getReturnType());

    bool isVoid = sig->getReturnType()->equal(ASTVoidType::get());

    if (emit_device && funcInfo->isCudaGlobal() && !isVoid) {
        argTypes.push_back(PointerType::getUnqual(returnType));
        returnType = Type::getVoidTy(context);
    }

    // Add arguments to LLVM function type
    while (args != nullptr) {
        std::shared_ptr<ASTArgNode> arg = args->getHead();
        argTypes.push_back(convertType(arg->getType()));
        args = args->getTail();
    }

    ftype = FunctionType::get(returnType, argTypes, false);

    Function *function = Function::Create(ftype, GlobalValue::ExternalLinkage, funcInfo->getName(), module.get());
    functions.set(funcInfo->getName(), function);

    return function;
}

void CodegenCtx::startFunction(std::string id) {
    this->function = getFunction(id);
    symbols.clear();
    blocks.clear();

    def_bblock = BasicBlock::Create(context, "defs", function, nullptr);
    def_builder = std::make_shared<IRBuilder<>>(def_bblock);

    first_bblock = createBlock();
    pushBlock(first_bblock);

    this->funcInfo = modInfo->getFunction(id);
    std::shared_ptr<ASTFunType> sig = funcInfo->getSignature();

    std::shared_ptr<ASTArgSeqNode> args = sig->getArgs();
    auto arg_iter = function->arg_begin();

    if (emit_device && funcInfo->isCudaGlobal() && !sig->getReturnType()->equal(ASTVoidType::get()))
        arg_iter++;

    // Map arguments to symbol table. Move arguments into alloca's functions
    while (args != nullptr) {
        std::shared_ptr<ASTArgNode> arg = args->getHead();

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
    return BasicBlock::Create(context, "L", function, nullptr);
}

void CodegenCtx::pushBlock(BasicBlock *block) {
    blocks.push_back(block);
    this->body_builder = std::make_shared<IRBuilder<>>(block);
}

void CodegenCtx::popBlock() {
    blocks.pop_back();
    BasicBlock *block = blocks.back();
    this->body_builder = std::make_shared<IRBuilder<>>(block);
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

std::shared_ptr<IRBuilder<>> CodegenCtx::getBuilder() {
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
