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

namespace Codegen {

class CodegenCtx {
private:

    // Whole module
    Module                    *module;       // LLVM module for all functions
    LLVMContext               &context;      // LLVM context
    bool                       emit_device;  // Should we emit GPU code
    SymbolTable<Function *>    functions;    // Mapping from function names to LLVM functions
    ModuleInfo                *modInfo;      // Information about module

    // Current function
    BasicBlock                *def_bblock;   // Locals definition block, assists with SSA
    BasicBlock                *first_bblock; // First block (after def block)
    IRBuilder<>               *def_builder;  // IRBuilder for def block
    SymbolTable<Value *>       symbols;      // Mapping from local symbols to LLVM values
    Function                  *function;     // LLVM function
    std::vector<BasicBlock *>  blocks;       // Stack of basic blocks
    FunctionInfo              *funcInfo;     // Information about current function

    // Current basic block
    IRBuilder<>               *body_builder; // IRBuilder for current block

    /**
     * @brief Insert return instructions in basic blocks that are missing a terminator.
     * We know that the function is well-formed due to the statics checks, so the only possibility
     * is that the (void) function did not have an explicit return statement. In the
     * latter case, insert an empty return statement. Also checks any successor blocks.
     *
     * @param[inout] visited Set of blocks already visited
     * @param[in]    bblock  Block to visit
     */
    void insertMissingReturns(std::unordered_set<BasicBlock *> & visited, BasicBlock *bblock);

public:

    /**
     * @brief Constructor
     *
     * @param[in] emit_device Whether to emit GPU code
     * @param[in] modInfo     Module information
     */
    CodegenCtx(bool emit_device, ModuleInfo *modInfo);

    /**
     * @brief Get module information
     */
    ModuleInfo *getModuleInfo();

    /**
     * @brief Get LLVM module
     */
    Module *getModule();

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
     * @brief Create an LLVM function for a FunctionInfo
     */
    Function *createFunction(FunctionInfo *funcInfo);

    /**
     * @brief Get a function by name
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
    FunctionInfo *getCurrentFunctionInfo();

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
    IRBuilder<> *getBuilder();

};

}

#endif
