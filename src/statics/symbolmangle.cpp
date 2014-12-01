/**
 * @file symbolmangle.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/symbolmangle.h>

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
}

void SymbolMangle::visitCallExp(std::shared_ptr<ASTCallExp> call) {
    if (internal.find(call->getId()) != internal.end())
        call->setId(mangleSymbol(call->getId()));

    ASTVisitor::visitCallExp(call);
}

void SymbolMangle::visitFunDecl(std::shared_ptr<ASTFunDecl> funDecl) {
    std::shared_ptr<FunctionInfo> funInfo = module->getFunction(funDecl->getName());

    if (funInfo->getLinkage() == ASTDeclNode::Internal) {
        std::string unmangled = funDecl->getName();
        std::string mangled = mangleSymbol(unmangled);

        internal.insert(unmangled);
        funDecl->setName(mangled);

        // TODO: this leaves the old names in the module
        funInfo->setName(mangled);
        module->addFunction(funInfo);
    }

    ASTVisitor::visitFunDecl(funDecl);
}
