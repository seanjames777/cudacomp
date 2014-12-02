/**
 * @file functioninfo.h
 *
 * @brief Information about a function
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __FUNCTIONINFO_H
#define __FUNCTIONINFO_H

#include <statics/symboltable.h>
#include <ast/type/asttypenode.h>
#include <ast/type/astfuntype.h>
#include <ast/decl/astdeclnode.h>

/**
 * @brief Information about a function, including its name, signature, and
 * local symbols' types.
 */
class FunctionInfo {
public:

    /**
     * @brief CUDA function usage flags
     */
    enum CudaUsage {
        Host = 1,
        Device = 2,
        Global = 4,
    };

private:

    std::shared_ptr<ASTFunType> signature;
    SymbolTable<std::shared_ptr<ASTTypeNode>> locals; // including arguments
    std::string name;
    enum ASTDeclNode::Linkage linkage;
    enum CudaUsage usage;

public:

    /**
     * @brief Constructor. Arguments will be added to the symbol table.
     *
     * @param[in] name      Function name
     * @param[in] signature Function signature
     * @param[in] linkage   Function linkage
     * @param[in] usage     CUDA function usage
     */
    FunctionInfo(std::string name, std::shared_ptr<ASTFunType> signature,
        enum ASTDeclNode::Linkage linkage, enum CudaUsage usage);

    /**
     * @brief Get function signature
     */
    std::shared_ptr<ASTFunType> getSignature();

    /**
     * @brief Get function name
     */
    std::string getName();

    /**
     * @brief Set function name
     */
    void setName(std::string name);

    /**
     * @brief Get function linkage
     */
     enum ASTDeclNode::Linkage getLinkage();

    /**
     * @brief Add a local symbol to the function
     *
     * @param[in] id   Symbol name
     * @param[in] type Symbol type
     */
    void addLocal(std::string id, std::shared_ptr<ASTTypeNode> type);

    /**
     * @brief Get CUDA function usage
     */
    enum CudaUsage getUsage();

    /**
     * @brief Get the type of a symbol in the function's body
     *
     * @param[in] id Symbol name
     */
    std::shared_ptr<ASTTypeNode> getLocalType(std::string id);

    /**
     * @brief Check whether a local symbol has been declared
     *
     * @param[in] id Symbol name
     */
    bool hasLocal(std::string id);

    /**
     * @brief Copy argument names/types to local symbols. This is deferred so
     * that the symbols can be renamed/type resolved before typechecking.
     */
    void copyArgumentsToLocals();

};

#endif
