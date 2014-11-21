/**
 * @file functioninfo.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/functioninfo.h>

FunctionInfo::FunctionInfo(std::string name, ASTFunType *signature)
    : name(name),
      signature(signature)
{
}

std::string FunctionInfo::getName() {
    return name;
}

ASTFunType *FunctionInfo::getSignature() {
    return signature;
}

ASTType *FunctionInfo::getLocalType(std::string id) {
    return locals.get(id);
}

void FunctionInfo::addLocal(std::string id, ASTType *type) {
    locals.set(id, type);
}

bool FunctionInfo::hasLocal(std::string id) {
    return locals.hasSymbol(id);
}
