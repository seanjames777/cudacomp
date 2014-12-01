/**
 * @file moduleinfo.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/moduleinfo.h>

ModuleInfo::ModuleInfo() {
}

void ModuleInfo::addFunction(std::shared_ptr<FunctionInfo> function) {
    functions.set(function->getName(), function);
}

std::shared_ptr<FunctionInfo> ModuleInfo::getFunction(std::string id) {
    if (!functions.hasSymbol(id))
        return nullptr;

    return functions.get(id);
}

void ModuleInfo::addType(std::string name, std::shared_ptr<ASTTypeNode> type) {
    typedefs.set(name, type);
}

std::shared_ptr<ASTTypeNode> ModuleInfo::getType(std::string id) {
    if (!typedefs.hasSymbol(id))
        return nullptr;

    return typedefs.get(id);
}

void ModuleInfo::addRecordType(std::string name, std::shared_ptr<ASTRecordType> type) {
    recordTypes.set(name,type);
}

std::shared_ptr<ASTRecordType> ModuleInfo::getRecordType(std::string id) {
    if (!recordTypes.hasSymbol(id))
        return nullptr;

    return recordTypes.get(id);
}
