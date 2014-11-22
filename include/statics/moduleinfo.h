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

/**
 * @brief Packages all of the information known about a module. A module contains
 * global variables, functions, struct declarations, type definitions, etc.
 */
class ModuleInfo {
private:

    SymbolTable<std::shared_ptr<FunctionInfo>> functions;

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

};

#endif
