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
    ASTArgSeqNode *args = signature->getArgs();

    while (args != NULL) {
        ASTArg *arg = args->getHead();
        addLocal(arg->getName(), arg->getType());
        args = args->getTail();
    }
}

std::string FunctionInfo::getName() {
    return name;
}

ASTFunType *FunctionInfo::getSignature() {
    return signature;
}

ASTTypeNode *FunctionInfo::getLocalType(std::string id) {
    return locals.get(id);
}

void FunctionInfo::addLocal(std::string id, ASTTypeNode *type) {
    locals.set(id, type);
}

bool FunctionInfo::hasLocal(std::string id) {
    return locals.hasSymbol(id);
}
