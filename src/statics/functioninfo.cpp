/**
 * @file functioninfo.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/functioninfo.h>

FunctionInfo::FunctionInfo(std::string name, std::shared_ptr<ASTFunType> signature,
    enum ASTDeclNode::Linkage linkage, bool cudaGlobal)
    : signature(signature),
      name(name),
      linkage(linkage),
      cudaGlobal(cudaGlobal)
{
}

std::string FunctionInfo::getName() {
    return name;
}

void FunctionInfo::setName(std::string name) {
    this->name = name;
}

enum ASTDeclNode::Linkage FunctionInfo::getLinkage() {
    return linkage;
}

std::shared_ptr<ASTFunType> FunctionInfo::getSignature() {
    return signature;
}

std::shared_ptr<ASTTypeNode> FunctionInfo::getLocalType(std::string id) {
    return locals.get(id);
}

bool FunctionInfo::isCudaGlobal() {
    return cudaGlobal;
}

void FunctionInfo::addLocal(std::string id, std::shared_ptr<ASTTypeNode> type) {
    locals.set(id, type);
}

bool FunctionInfo::hasLocal(std::string id) {
    return locals.hasSymbol(id);
}

void FunctionInfo::copyArgumentsToLocals() {
    std::shared_ptr<ASTArgSeqNode> args = signature->getArgs();

    while (args != nullptr) {
        std::shared_ptr<ASTArgNode> arg = args->getHead();
        addLocal(arg->getName(), arg->getType());
        args = args->getTail();
    }
}
