/**
 * @file moduleinfo.h
 *
 * @brief Information about a module, including function signatures, type
 * definitions, etc.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __MODULEINFO_H
#define __MODULEINFO_H

#include <statics/symboltable.h>
#include <statics/functioninfo.h>

class ModuleInfo {
private:

    SymbolTable<FunctionInfo *> functions;

public:

    ModuleInfo();

    void addFunction(std::string id, FunctionInfo *function);

    FunctionInfo *getFunction(std::string id);

};

#endif
