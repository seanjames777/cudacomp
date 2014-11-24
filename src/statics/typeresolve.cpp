/**
 * @file typeresolve.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/typeresolve.h>
#include <ast/expr/astintegerexp.h>
#include <ast/expr/astbinopexp.h>
#include <ast/astseqnode.h>
#include <ast/stmt/astreturnstmt.h>
#include <ast/expr/astidentifierexp.h>
#include <ast/stmt/astvardeclstmt.h>
#include <ast/stmt/astassignstmt.h>
#include <ast/type/astintegertype.h>
#include <ast/expr/astunopexp.h>
#include <ast/type/astbooleantype.h>
#include <ast/stmt/astscopestmt.h>
#include <ast/stmt/astifstmt.h>
#include <ast/expr/astbooleanexp.h>
#include <ast/decl/astfundecl.h>
#include <ast/expr/astcallexp.h>
#include <ast/type/astvoidtype.h>
#include <ast/type/astptrtype.h>
#include <ast/stmt/astexprstmt.h>
#include <ast/decl/asttypedecl.h>
#include <ast/type/astidtype.h>

namespace Statics {

std::shared_ptr<ASTTypeNode> resolveType(std::shared_ptr<ModuleInfo> module, std::shared_ptr<ASTTypeNode> type) {
    // Type definitions can only introduce one level of indirection. So, a newly defined type is
    // either a basic type like int or bool, or a single indirection into the existing types.

    // Resolve the 'to' type for pointers. TODO: test this when language support arrives.
    if (std::shared_ptr<ASTPtrType> ptr_type = std::dynamic_pointer_cast<ASTPtrType>(type)) {
        ptr_type->setToType(resolveType(module, ptr_type->getToType()));
        return ptr_type;
    }
    // Resolve the element type for arrays. TODO: test this
    else if (std::shared_ptr<ASTArrType> arr_type = std::dynamic_pointer_cast<ASTArrType>(type)) {
        arr_type->setElemType(resolveType(module, arr_type->getElemType()));
        return arr_type;
    }
    // ID type. Look up the type, which should already be resolved.
    else if (std::shared_ptr<ASTIdType> id_type = std::dynamic_pointer_cast<ASTIdType>(type)) {
        std::shared_ptr<ASTTypeNode> resolved = module->getType(id_type->getId());

        // The type must be declared already.
        if (!resolved)
            throw UndeclaredTypeException(id_type->getId());

        return resolved;
    }
    // Otherwise it's a basic type already
    else
        return type;
}

void typeresolve_stmts(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<ASTStmtSeqNode> seq_node)
{
    while (seq_node != nullptr) {
        typeresolve_stmt(mod, seq_node->getHead());
        seq_node = seq_node->getTail();
    }
}

void typeresolve_stmt(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<ASTStmtNode> head)
{
    // Variable declaration. Resolve the declared type.
    if (std::shared_ptr<ASTVarDeclStmt> decl_stmt = std::dynamic_pointer_cast<ASTVarDeclStmt>(head))
        decl_stmt->setType(resolveType(mod, decl_stmt->getType()));
    // Scope statement. Resolve the body
    else if (std::shared_ptr<ASTScopeStmt> scope_node = std::dynamic_pointer_cast<ASTScopeStmt>(head)) {
        if (scope_node->getBody())
            typeresolve_stmts(mod, scope_node->getBody());
    }
    // If statement. Resolve branches.
    else if (std::shared_ptr<ASTIfStmt> if_node = std::dynamic_pointer_cast<ASTIfStmt>(head)) {
        typeresolve_stmts(mod, if_node->getTrueStmt());

        if (if_node->getFalseStmt())
            typeresolve_stmts(mod, if_node->getFalseStmt());
    }
    // While statement. Resolve body.
    else if (std::shared_ptr<ASTWhileStmt> while_node = std::dynamic_pointer_cast<ASTWhileStmt>(head)) {
        typeresolve_stmts(mod, while_node->getBody());
    }

    // TODO: need to handle expressions now for alloc_array
}

void typeresolve_tops(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<ASTDeclSeqNode> seq_node)
{
    while (seq_node != nullptr) {
        typeresolve_top(mod, seq_node->getHead());
        seq_node = seq_node->getTail();
    }
}

void typeresolve_top(
    std::shared_ptr<ModuleInfo> mod,
    std::shared_ptr<ASTDeclNode> node)
{
    // Function definition
    if (std::shared_ptr<ASTFunDecl> funDefn = std::dynamic_pointer_cast<ASTFunDecl>(node)) {
        std::shared_ptr<ASTFunType> fun_type = funDefn->getSignature();

        // Resolve the return type
        fun_type->setReturnType(resolveType(mod, fun_type->getReturnType()));

        std::shared_ptr<ASTArgSeqNode> args = fun_type->getArgs();

        // Resolve each argument's type
        while (args != nullptr) {
            std::shared_ptr<ASTArgNode> arg = args->getHead();
            arg->setType(resolveType(mod, arg->getType()));
            args = args->getTail();
        }

        // Resolve the body
        typeresolve_stmts(mod, funDefn->getBody());
    }
    // Type definition
    else if (std::shared_ptr<ASTTypeDecl> typeDefn = std::dynamic_pointer_cast<ASTTypeDecl>(node)) {
        // Types must have a new name
        if (mod->getType(typeDefn->getName()) != nullptr)
            throw RedeclaredTypeException(typeDefn->getName());

        // TODO: type cannot be defined in terms of itself
        // TODO: inconsistency between getName() and getId() all over the place

        // Add the new type to the mapping
        mod->addType(typeDefn->getName(), resolveType(mod, typeDefn->getType()));
    }
}

};
