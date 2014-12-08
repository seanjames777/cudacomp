/**
 * @file symbolmangle.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/symbolmangle.h>
#include <sstream>
#include <unordered_set>

std::string SymbolMangle::mangleSymbol(std::string name) {
    std::stringstream ss;
    ss << args->symbol_prefix << name;
    return ss.str();
}

SymbolMangle::SymbolMangle(std::shared_ptr<ModuleInfo> module)
    : module(module),
      args(getOptions())
{
}

void SymbolMangle::run(std::shared_ptr<ASTDeclSeqNode> ast) {
    visitNode(ast);

    // Rename FunctionInfo's
    std::unordered_set<std::shared_ptr<FunctionInfo>> internal;

    auto & functions = module->getFunctions();

    for (auto it = functions.begin(); it != functions.end();) {
        auto old_it = it++;

        if (old_it->second->getLinkage() == ASTDeclNode::Internal) {
            internal.insert(old_it->second);
            functions.erase(old_it);
        }
    }

    for (auto func : internal) {
        func->setName(mangleSymbol(func->getName()));
        functions.set(func->getName(), func);
    }
}

void SymbolMangle::visitCallExp(std::shared_ptr<ASTCallExp> call) {
    // Rename calls to internal functions

    if (module->getFunction(call->getId())->getLinkage() == ASTDeclNode::Internal)
        call->setId(mangleSymbol(call->getId()));

    ASTVisitor::visitCallExp(call);
}

void SymbolMangle::visitFunDecl(std::shared_ptr<ASTFunDecl> funDecl) {
    // Rename internal function declarations

    std::shared_ptr<FunctionInfo> funInfo = module->getFunction(funDecl->getName());

    if (funInfo->getLinkage() == ASTDeclNode::Internal)
        funDecl->setName(mangleSymbol(funDecl->getName()));

    ASTVisitor::visitFunDecl(funDecl);
}
