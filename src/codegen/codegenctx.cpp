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
      alloc_array(nullptr),
      alloc(nullptr),
      alloc_device(nullptr),
      cpy_h2d(nullptr),
      cpy_d2h(nullptr),
      invoke_kernel(nullptr),
      div_check(nullptr),
      shift_check(nullptr),
      arr_check(nullptr),
      deref_check(nullptr),
      fassert(nullptr),
      def_bblock(nullptr),
      first_bblock(nullptr),
      def_builder(nullptr),
      function(nullptr),
      funcInfo(nullptr),
      body_builder(nullptr)
{
    module = std::make_shared<Module>("", context);
    layout = std::make_shared<DataLayout>(module.get());
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

Function *CodegenCtx::getAlloc() {
    // Only declare it if it's used, to make small programs simpler
    if (!alloc) {
        // Construct a declaration of the runtime's alloc function
        std::vector<Type *> argTypes;
        argTypes.push_back(Type::getInt32Ty(context));

        FunctionType *ftype = FunctionType::get(PointerType::getUnqual(Type::getInt8Ty(context)), argTypes, false);
        alloc = Function::Create(ftype, GlobalValue::ExternalLinkage, "_rt_alloc", module.get());

    }
    return alloc;
}

Function *CodegenCtx::getAllocDevice() {
    // Only declare it if it's used, to make small programs simpler
    if (!alloc_device) {
        // Construct a declaration of the runtime's alloc_device function
        std::vector<Type *> argTypes;
        argTypes.push_back(Type::getInt32Ty(context));

        FunctionType *ftype = FunctionType::get(PointerType::getUnqual(Type::getInt8Ty(context)), argTypes, false);
        alloc_device = Function::Create(ftype, GlobalValue::ExternalLinkage, "_rt_alloc_device", module.get());
    }

    return alloc_device;
}

Function *CodegenCtx::getCopyHostToDevice() {
    // Only declare it if it's used, to make small programs simpler
    if (!cpy_h2d) {
        // Construct a declaration of the runtime's cpy_h2d function
        std::vector<Type *> argTypes;
        argTypes.push_back(PointerType::getUnqual(Type::getInt8Ty(context)));
        argTypes.push_back(PointerType::getUnqual(Type::getInt8Ty(context)));
        argTypes.push_back(Type::getInt32Ty(context));

        FunctionType *ftype = FunctionType::get(Type::getVoidTy(context), argTypes, false);
        cpy_h2d = Function::Create(ftype, GlobalValue::ExternalLinkage, "_rt_cpy_h2d", module.get());
    }

    return cpy_h2d;
}

Function *CodegenCtx::getCopyDeviceToHost() {
    // Only declare it if it's used, to make small programs simpler
    if (!cpy_d2h) {
        // Construct a declaration of the runtime's cpy_d2h function
        std::vector<Type *> argTypes;
        argTypes.push_back(PointerType::getUnqual(Type::getInt8Ty(context)));
        argTypes.push_back(PointerType::getUnqual(Type::getInt8Ty(context)));
        argTypes.push_back(Type::getInt32Ty(context));

        FunctionType *ftype = FunctionType::get(Type::getVoidTy(context), argTypes, false);
        cpy_d2h = Function::Create(ftype, GlobalValue::ExternalLinkage, "_rt_cpy_d2h", module.get());
    }

    return cpy_d2h;
}

Function *CodegenCtx::getInvokeKernel() {
    // Only declare it if it's used, to make small programs simpler
    if (!invoke_kernel) {
        // Construct a declaration of the runtime's invoke_kernel function
        std::vector<Type *> argTypes;
        argTypes.push_back(PointerType::getUnqual(Type::getInt8Ty(context)));
        argTypes.push_back(PointerType::getUnqual(PointerType::getUnqual(Type::getInt8Ty(context))));

        FunctionType *ftype = FunctionType::get(Type::getVoidTy(context), argTypes, false);
        invoke_kernel = Function::Create(ftype, GlobalValue::ExternalLinkage, "_rt_invoke_kernel", module.get());
    }

    return invoke_kernel;
}

Function *CodegenCtx::getDivCheck() {
    // Only declare it if it's used, to make small programs simpler
    if (!div_check) {
        // Construct a declaration of the runtime's div_check function
        std::vector<Type *> argTypes;
        argTypes.push_back(Type::getInt32Ty(context));
        argTypes.push_back(Type::getInt32Ty(context));

        FunctionType *ftype = FunctionType::get(Type::getVoidTy(context), argTypes, false);
        div_check = Function::Create(ftype, GlobalValue::ExternalLinkage, "_rt_div_check", module.get());
    }

    return div_check;
}

Function *CodegenCtx::getShiftCheck() {
    // Only declare it if it's used, to make small programs simpler
    if (!shift_check) {
        // Construct a declaration of the runtime's shift_check function
        std::vector<Type *> argTypes;
        argTypes.push_back(Type::getInt32Ty(context));

        FunctionType *ftype = FunctionType::get(Type::getVoidTy(context), argTypes, false);
        shift_check = Function::Create(ftype, GlobalValue::ExternalLinkage, "_rt_shift_check", module.get());
    }

    return shift_check;
}

Function *CodegenCtx::getArrBoundsCheck() {
    // Only declare it if it's used, to make small programs simpler
    if (!arr_check) {
        // Construct a declaration of the runtime's arr_check function
        std::vector<Type *> argTypes;
        argTypes.push_back(PointerType::getUnqual(Type::getInt8Ty(context)));
        argTypes.push_back(Type::getInt32Ty(context));

        FunctionType *ftype = FunctionType::get(Type::getVoidTy(context), argTypes, false);
        arr_check = Function::Create(ftype, GlobalValue::ExternalLinkage, "_rt_array_bounds_check", module.get());
    }

    return arr_check;
}

Function *CodegenCtx::getDerefCheck() {
    // Only declare it if it's used, to make small programs simpler
    if (!deref_check) {
        // Construct a declaration of the runtime's deref_check function
        std::vector<Type *> argTypes;
        argTypes.push_back(PointerType::getUnqual(Type::getInt8Ty(context)));

        FunctionType *ftype = FunctionType::get(Type::getVoidTy(context), argTypes, false);
        deref_check = Function::Create(ftype, GlobalValue::ExternalLinkage, "_rt_deref_check", module.get());
    }

    return deref_check;
}

Function *CodegenCtx::getAssert() {
    // Only declare it if it's used, to make small programs simpler
    if (!fassert) {
        // Construct a declaration of the runtime's fassert function
        std::vector<Type *> argTypes;
        argTypes.push_back(Type::getInt1Ty(context));

        FunctionType *ftype = FunctionType::get(Type::getVoidTy(context), argTypes, false);
        fassert = Function::Create(ftype, GlobalValue::ExternalLinkage, "_rt_assert", module.get());
    }

    return fassert;
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
    else {
        // Default to host machine's triple
        module->setTargetTriple(llvm::sys::getDefaultTargetTriple());
    }

    PassManager pm;

    // Replace memory operands with registers where possible
    pm.add(createPromoteMemoryToRegisterPass());

    // Perform some simple optimizations just to clean up the output IR
    // TODO

    // pm.add(createPrintModulePass(outs));

    pm.run(*module);

    CCArgs *args = getOptions();

    // Finally, print the result to the output stream
    raw_os_ostream outs(out);

    if (args->emit_text)
        outs << *module;
    else
        WriteBitcodeToFile(module.get(), outs);
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
    Type *returnType = convertType(sig->getReturnType(), this);

    bool isVoid = sig->getReturnType()->equal(ASTVoidType::get());

    if (emit_device && (funcInfo->getUsage() & FunctionInfo::Global) && !isVoid) {
        argTypes.push_back(PointerType::getUnqual(returnType));
        returnType = Type::getVoidTy(context);
    }

    // Add arguments to LLVM function type
    while (args != nullptr) {
        std::shared_ptr<ASTArgNode> arg = args->getHead();
        argTypes.push_back(convertType(arg->getType(), this));
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

    if (emit_device && (funcInfo->getUsage() & FunctionInfo::Global) && !sig->getReturnType()->equal(ASTVoidType::get()))
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

    NamedMDNode *nvvm = module->getOrInsertNamedMetadata("nvvm.annotations");
    nvvm->addOperand(node);
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
        Value *instr = def_builder->CreateAlloca(convertType(funcInfo->getLocalType(id), this));
        symbols.set(id, instr);
        return instr;
    }

    return symbols.get(id);
}

Value *CodegenCtx::createTemp(Type *type) {
    return def_builder->CreateAlloca(type);
}

Type *CodegenCtx::getRecordType(std::string name) {
    if (records.hasSymbol(name))
        return records.get(name);

    // The record has not been declared, so implicitly declare an opaque type
    StructType *type = StructType::create(context, name);
    records.set(name, type);
    return type;

}

void CodegenCtx::createRecord(std::shared_ptr<ASTRecordType> recordInfo){
    // TODO : evaluate possibility of undesirable name conflicts between "struct foo" and named type foo
    std::string name = recordInfo->getId();
    // Add opaque type for the recursive case
    StructType *type = StructType::create(context, name);
    records.set(name, type);

    std::vector<Type *> elems;
    std::shared_ptr<ASTArgSeqNode> fields = recordInfo->getFields();
    while (fields != nullptr) {
        std::shared_ptr<ASTTypeNode> field_type = fields->getHead()->getType();
        elems.push_back(convertType(field_type, this));
        fields = fields->getTail();
    }
    type->setBody(elems);
}

unsigned long CodegenCtx::getSize(std::shared_ptr<ASTTypeNode> t) {
    Type * tau = convertType(t, this);
    return layout->getTypeSizeInBits(tau) / 8;
}

unsigned long CodegenCtx::getAlignedSize(std::shared_ptr<ASTTypeNode> t) {
    Type * tau = convertType(t, this);
    return layout->getTypeAllocSize(tau);
}

}
