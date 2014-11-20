/**
 * @file functioninfo.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/functioninfo.h>

FunctionInfo::FunctionInfo(ASTFunType *signature)
    : signature(signature)
{
}

ASTType *FunctionInfo::getLocalType(std::string id) {
    return locals.get(id);
}

void FunctionInfo::setLocalType(std::string id, ASTType *type) {
    locals.set(id, type);
}
