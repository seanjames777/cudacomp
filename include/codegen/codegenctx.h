/**
 * @file codegenctx.h
 *
 * @brief Code generation context
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __CODEGENCTX_H
#define __CODEGENCTX_H

#include <defs.h>
#include <statics/symboltable.h>
#include <statics/functioninfo.h>
#include <statics/moduleinfo.h>
#include <options.h>

#include <llvm/IR/DataLayout.h>
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
#include <llvm/Support/Host.h>
#include <llvm/ADT/Triple.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Bitcode/ReaderWriter.h>

using namespace llvm;

namespace Codegen {

class CodegenCtx {
private:

    // Whole module
    std::shared_ptr<Module>        module;        // LLVM module for all functions
    LLVMContext                   &context;       // LLVM context
    std::shared_ptr<DataLayout>    layout;        // LLVM data layout
    bool                           emit_device;   // Should we emit GPU code
    SymbolTable<Function *>        functions;     // Mapping from function names to LLVM functions
    SymbolTable<StructType *>      records;       // Mapping from struct names to LLVM types
    std::shared_ptr<ModuleInfo>    modInfo;       // Information about module
    Function                      *alloc_array;   // Runtime alloc_array function
    Function                      *alloc;         // Runtime alloc function
    Function                      *alloc_device;  // Allocate untyped device memory
    Function                      *cpy_h2d;       // Copy from host to device
    Function                      *cpy_d2h;       // Copy from device to host
    Function                      *invoke_kernel; // Invoke a kernel
    Function                      *div_check;     // Runtime division safety check
    Function                      *shift_check;   // Runtime shift safety check
    Function                      *arr_check;     // Array bounds check
    Function                      *deref_check;   // Array bounds check
    Function                      *fassert;       // Runtime assertion

    // Current function
    BasicBlock                    *def_bblock;    // Locals definition block, assists with SSA
    BasicBlock                    *first_bblock;  // First block (after def block)
    std::shared_ptr<IRBuilder<>>   def_builder;   // IRBuilder for def block
    SymbolTable<Value *>           symbols;       // Mapping from local symbols to LLVM values
    Function                      *function;      // LLVM function
    std::vector<BasicBlock *>      blocks;        // Stack of basic blocks
    std::shared_ptr<FunctionInfo>  funcInfo;      // Information about current function

    // Current basic block
    std::shared_ptr<IRBuilder<>>   body_builder; // IRBuilder for current block

public:

    /**
     * @brief Constructor
     *
     * @param[in] emit_device Whether to emit GPU code
     * @param[in] modInfo     Module information
     */
    CodegenCtx(bool emit_device, std::shared_ptr<ModuleInfo> modInfo);

    /**
     * @brief Get the 'alloc_array' runtime function
     */
    Function *getAllocArray();

    /**
     * @brief Get the 'alloc' runtime function
     */
    Function *getAlloc();

    /**
     * @brief Get the 'alloc_device' runtime function
     */
    Function *getAllocDevice();

    /**
     * @brief Get the 'alloc_array' runtime function // TODO
     */
    Function *getCopyHostToDevice();

    /**
     * @brief Get the 'alloc_array' runtime function
     */
    Function *getCopyDeviceToHost();

    /**
     * @brief Get the 'alloc_array' runtime function
     */
    Function *getInvokeKernel();

    /*
     * @brief Get the 'div_check' runtime function
     */
    Function *getDivCheck();

    /**
     * @brief Get the 'shift_check' runtime function
     */
    Function *getShiftCheck();

    /**
     * @brief Get the 'arr_bounds_check' runtime function
     */
    Function *getArrBoundsCheck();

    /**
     * @brief Get the 'deref_check' runtime function
     */
    Function *getDerefCheck();

    /**
     * @brief Get the 'assert' runtime function
     */
    Function *getAssert();

    /**
     * @brief Get module information
     */
    std::shared_ptr<ModuleInfo> getModuleInfo();

    /**
     * @brief Get LLVM module
     */
    std::shared_ptr<Module> getModule();

    /**
     * @brief Get whether we should emit GPU code
     */
    bool getEmitDevice();

    /**
     * @brief Get LLVM context
     */
    LLVMContext & getContext();

    /**
     * @brief Emit the module to a stream
     */
    void emit(std::ostream & out);

    /**
     * @brief Create an LLVM function for a FunctionInfo. The pointer will be
     * owned by the LLVM module.
     */
    Function *createFunction(std::shared_ptr<FunctionInfo> funcInfo);

    /**
     * @brief Get a function by name. It must have been created already through
     * createFunction().
     */
    Function *getFunction(std::string id);

    /**
     * @brief Start generating code for a function. Clears the basic block
     * stack, etc.
     */
    void startFunction(std::string id);

    /**
     * @brief Get information about current function
     */
    std::shared_ptr<FunctionInfo> getCurrentFunctionInfo();

    /**
     * @brief Get the current LLVM function
     */
    Function *getCurrentFunction();

    /**
     * @brief Get or create a value for a local symbol name in the current
     * function.
     */
    Value *getOrCreateSymbol(std::string symbol);

    /**
     * @brief Create a new temporary value which will be alloca'd in the
     * definition block for mem2reg to find.
     */
    Value *createTemp(Type *type);

    /**
     * @brief Mark a function as a GPU kernel
     */
    void markKernel(Function *kernel);

    /**
     * @brief Finish the current function and write IR to an output stream
     */
    void finishFunction();

    /**
     * @brief Create a new basic block
     */
    BasicBlock *createBlock();

    /**
     * @brief Make a basic block the "current" basic block and push it into the
     * stack of basic blocks. Updates the builder returned by getBuilder().
     */
    void pushBlock(BasicBlock *block);

    /**
     * @brief Pop the "current" basic block off of the stack and make the
     * top block the new "current" block.
     */
    void popBlock();

    /**
     * @brief Get an IR builder for the "current" basic block
     */
    std::shared_ptr<IRBuilder<>> getBuilder();

    /**
     * @brief Declare an LLVM record type. It may be retrieved later with the getRecordType function.
     */
    void createRecord(std::shared_ptr<ASTRecordType> recordInfo);


    /**
     * @brief Gets the LLVM type for the record with the given name
     */
    Type *getRecordType(std::string name);

    /**
     * @brief Gets the bits needed to hold a type, without padding.
     */
    unsigned long getSize(std::shared_ptr<ASTTypeNode> t);

    /**
     * @brief Gets the bits needed to hold a type, with padding.
     */
    unsigned long getAlignedSize(std::shared_ptr<ASTTypeNode> t);
};

}

#endif
