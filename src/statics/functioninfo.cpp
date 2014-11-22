/**
 * @file functioninfo.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/functioninfo.h>

FunctionInfo::FunctionInfo(std::string name, std::shared_ptr<ASTFunType> signature)
    : name(name),
      signature(signature)
{
    std::shared_ptr<ASTArgSeqNode> args = signature->getArgs();

    while (args != NULL) {
        std::shared_ptr<ASTArgNode> arg = args->getHead();
        addLocal(arg->getName(), arg->getType());
        args = args->getTail();
    }
}

std::string FunctionInfo::getName() {
    return name;
}

std::shared_ptr<ASTFunType> FunctionInfo::getSignature() {
    return signature;
}

std::shared_ptr<ASTTypeNode> FunctionInfo::getLocalType(std::string id) {
    return locals.get(id);
}

void FunctionInfo::addLocal(std::string id, std::shared_ptr<ASTTypeNode> type) {
    locals.set(id, type);
}

bool FunctionInfo::hasLocal(std::string id) {
    return locals.hasSymbol(id);
}
