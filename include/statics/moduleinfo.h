/**
 * @file moduleinfo.h
 *
 * @brief Information about a module
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __MODULEINFO_H
#define __MODULEINFO_H

#include <statics/symboltable.h>
#include <statics/functioninfo.h>
#include <ast/type/astrecordtype.h>

/**
 * @brief Packages all of the information known about a module. A module contains
 * global variables, functions, struct declarations, type definitions, etc.
 */
class ModuleInfo {
private:

    SymbolTable<std::shared_ptr<FunctionInfo>> functions;
    SymbolTable<std::shared_ptr<ASTTypeNode>> typedefs;
    SymbolTable<std::shared_ptr<ASTRecordType>> recordTypes;

public:

    /**
     * @brief Constructor
     */
    ModuleInfo();

    /**
     * @brief Add a function to the module
     *
     * @param[in] function Function information
     */
    void addFunction(std::shared_ptr<FunctionInfo> function);

    /**
     * @brief Get information about a function
     *
     * @param[in] id Function name
     */
    std::shared_ptr<FunctionInfo> getFunction(std::string id);

    /**
     * @brief Add a type to the module
     *
     * @param[in] name Name of type
     * @param[in] type Actual type
     */
    void addType(std::string name, std::shared_ptr<ASTTypeNode> type);

    /**
     * @brief Get the actual type for a type name
     *
     * @param[in] name Name of type
     */
    std::shared_ptr<ASTTypeNode> getType(std::string id);

    /**
     * @brief Add a record declaration to the module
     *
     * @param[in] name Name of the record
     * @param[in] type The type object containing field information
     */
    void addRecordType(std::string name, std::shared_ptr<ASTRecordType> type);

    /**
     * @brief Get the type object for a record
     *
     * @param[in] name Name of the record
     */
    std::shared_ptr<ASTRecordType> getRecordType(std::string id);
};

#endif
