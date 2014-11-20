/**
 * @file moduleinfo.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/moduleinfo.h>

ModuleInfo::ModuleInfo() {
}

void ModuleInfo::addFunction(std::string id, FunctionInfo *function) {
    functions.set(id, function);
}

FunctionInfo *ModuleInfo::getFunction(std::string id) {
    if (!functions.hasSymbol(id))
        return NULL;

    return functions.get(id);
}
