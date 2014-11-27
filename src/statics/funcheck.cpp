/**
 * @file funcheck.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/funcheck.h>

namespace Statics {

FunCheck::FunCheck(std::shared_ptr<ModuleInfo> module)
    : module(module)
{
}

void FunCheck::run(std::shared_ptr<ASTDeclSeqNode> ast) {
    // Visit the AST
    visitDeclSeqNode(ast);

    // Find the functions which were called but never defined
    idset undef;

    std::set_difference(
        called.begin(), called.end(),
        defined.begin(), defined.end(),
        std::inserter(undef, undef.end()));

    for (auto id : undef)
        throw UndefinedFunctionException(id);
}

void FunCheck::visitCallExp(std::shared_ptr<ASTCallExp> call_exp) {
    std::shared_ptr<FunctionInfo> call_func = module->getFunction(call_exp->getId());

    // Function must have been declared
    if (!call_func)
        throw UndeclaredFunctionException(call_exp->getId());

    // Don't require definitions for externally defined functions
    if (call_func->getLinkage() == ASTDeclNode::Internal)
        called.insert(call_exp->getId());

    ASTVisitor::visitCallExp(call_exp);
}

void FunCheck::visitFunDecl(std::shared_ptr<ASTFunDecl> funDefn) {
    // Check for an existing declaration
    std::shared_ptr<FunctionInfo> funInfo = module->getFunction(funDefn->getName());

    // Make sure external functions are not defined
    if (funDefn->getLinkage() == ASTDeclNode::External && funDefn->isDefn())
        throw ExternalFunctionDefinedException(funDefn->getName());

    if (funInfo) {
        std::shared_ptr<ASTFunType> curr_sig = funInfo->getSignature();
        std::shared_ptr<ASTFunType> new_sig = funDefn->getSignature();

        // If there is an existing declaration, it must match this one
        if (!curr_sig->equal(new_sig))
            throw IncorrectSignatureException(funDefn->getName());

        if (funInfo->getLinkage() != funDefn->getLinkage())
            throw IncorrectLinkageException(funDefn->getName());

        // If there is a definition, the function may not be defined already
        if (funDefn->isDefn() && defined.find(funDefn->getName()) != defined.end())
            throw RedefinedFunctionException(funDefn->getName());
    }
    else {
        // Add the new function to the module
        funInfo = std::make_shared<FunctionInfo>(funDefn->getName(),
            funDefn->getSignature(), funDefn->getLinkage(),
            funDefn->getName() == "_cc_main");
        module->addFunction(funInfo);
    }

    // If there is a body, mark the function as defined
    if (funDefn->isDefn())
        defined.insert(funDefn->getName());

    ASTVisitor::visitFunDecl(funDefn);
}

}
