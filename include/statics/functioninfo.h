/**
 * @file functioninfo.h
 *
 * @brief Information about a function, including its signature and local
 * variable type information.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __FUNCTIONINFO_H
#define __FUNCTIONINFO_H

#include <statics/symboltable.h>
#include <ast/type/asttypenode.h>
#include <ast/type/astfuntype.h>

/**
 * @brief Information about a function, including its name, signature, and
 * local symbols' types.
 */
class FunctionInfo {
private:

    ASTFunType *signature;
    SymbolTable<ASTTypeNode *> locals; // including arguments
    std::string name;

public:

    /**
     * @brief Constructor. Arguments will be added to the symbol table.
     *
     * @param[in] name      Function name
     * @param[in] signature Function signature
     */
    FunctionInfo(std::string name, ASTFunType *signature);

    /**
     * @brief Get function signature
     */
    ASTFunType *getSignature();

    /**
     * @brief Get function name
     */
    std::string getName();

    /**
     * @brief Add a local symbol to the function
     *
     * @param[in] id   Symbol name
     * @param[in] type Symbol type
     */
    void addLocal(std::string id, ASTTypeNode *type);

    /**
     * @brief Get the type of a symbol in the function's body
     *
     * @param[in] id Symbol name
     */
    ASTTypeNode *getLocalType(std::string id);

    /**
     * @brief Check whether a local symbol has been declared
     *
     * @param[in] id Symbol name
     */
    bool hasLocal(std::string id);

};

#endif
