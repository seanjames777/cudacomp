/**
 * @file funcheck.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/funcheck.h>

namespace Statics {

void funcheck_exp(
    std::shared_ptr<ModuleInfo> mod,
    idset & called,
    std::shared_ptr<ASTExpNode> node)
{
    if (std::shared_ptr<ASTCallExp> call_exp = std::dynamic_pointer_cast<ASTCallExp>(node)) {
        std::shared_ptr<FunctionInfo> call_func = mod->getFunction(call_exp->getId());

        // Function must have been declared
        if (!call_func)
            throw UndeclaredFunctionException(call_exp->getId());

        called.insert(call_exp->getId());
    }
}

void funcheck_stmts(
    std::shared_ptr<ModuleInfo> mod,
    idset & called,
    std::shared_ptr<ASTStmtSeqNode> seq_node)
{
    while (seq_node != nullptr) {
        funcheck_stmt(mod, called, seq_node->getHead());
        seq_node = seq_node->getTail();
    }
}

void funcheck_stmt(
    std::shared_ptr<ModuleInfo> mod,
    idset & called,
    std::shared_ptr<ASTStmtNode> head)
{
    if (std::shared_ptr<ASTVarDeclStmt> decl_stmt = std::dynamic_pointer_cast<ASTVarDeclStmt>(head)) {
        if (decl_stmt->getExp())
            funcheck_exp(mod, called, decl_stmt->getExp());
    }
    else if (std::shared_ptr<ASTVarDefnStmt> defn_stmt = std::dynamic_pointer_cast<ASTVarDefnStmt>(head))
        funcheck_exp(mod, called, defn_stmt->getExp());
    else if (std::shared_ptr<ASTReturnStmt> ret_node = std::dynamic_pointer_cast<ASTReturnStmt>(head)) {
        if (ret_node->getExp())
            funcheck_exp(mod, called, ret_node->getExp());
    }
    else if (std::shared_ptr<ASTScopeStmt> scope_node = std::dynamic_pointer_cast<ASTScopeStmt>(head)) {
        if (scope_node->getBody())
            funcheck_stmts(mod, called, scope_node->getBody());
    }
    else if (std::shared_ptr<ASTIfStmt> if_node = std::dynamic_pointer_cast<ASTIfStmt>(head)) {
        funcheck_stmts(mod, called, if_node->getTrueStmt());

        if (if_node->getFalseStmt())
            funcheck_stmts(mod, called, if_node->getFalseStmt());
    }
    else if (std::shared_ptr<ASTExprStmt> exp_stmt = std::dynamic_pointer_cast<ASTExprStmt>(head))
        funcheck_exp(mod, called, exp_stmt->getExp());
    else
        throw new ASTMalformedException();
}

void funcheck_tops(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<ASTDeclSeqNode> seq_node)
{
    idset called;
    idset defined;

    while (seq_node != nullptr) {
        funcheck_top(mod, called, defined, seq_node->getHead());
        seq_node = seq_node->getTail();
    }

    // Find the functions which were called but never defined
    idset undef;

    std::set_difference(
        called.begin(), called.end(),
        defined.begin(), defined.end(),
        std::inserter(undef, undef.end()));

    for (auto id : undef)
        throw UndefinedFunctionException(id);
}

void funcheck_top(
    std::shared_ptr<ModuleInfo> mod,
    idset & called,
    idset & defined,
    std::shared_ptr<ASTDeclNode> node)
{
    if (std::shared_ptr<ASTFunDecl> funDefn = std::dynamic_pointer_cast<ASTFunDecl>(node)) {
        // Rules:
        //   - Functions may be declared more than once, but may only be defined once
        //   - Functions must be declared before they can be called
        //   - Any function that is called must be defined
        //   - All declarations of a function must have the same signature

        // Check for an existing declaration
        std::shared_ptr<FunctionInfo> funInfo = mod->getFunction(funDefn->getName());

        if (funInfo) {
            std::shared_ptr<ASTFunType> curr_sig = funInfo->getSignature();
            std::shared_ptr<ASTFunType> new_sig = funDefn->getSignature();

            // If there is an existing declaration, it must match this one
            if (!curr_sig->equal(new_sig))
                throw IncorrectSignatureException(funDefn->getName());

            // If there is a definition, the function may not be defined already
            if (funDefn->getBody() && defined.find(funDefn->getName()) != defined.end())
                throw RedefinedFunctionException(funDefn->getName());
        }
        else {
            // Add the new function to the module
            funInfo = std::make_shared<FunctionInfo>(funDefn->getName(), funDefn->getSignature());
            mod->addFunction(funInfo);
        }

        // If there is a body, mark the function as defined and collect function calls from the
        // body.
        if (funDefn->getBody()) {
            defined.insert(funDefn->getName());
            funcheck_stmts(mod, called, funDefn->getBody());
        }
    }
}

};
