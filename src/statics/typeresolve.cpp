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

TypeResolve::TypeResolve(std::shared_ptr<ModuleInfo> module)
    : module(module)
{
}

std::shared_ptr<ASTTypeNode> TypeResolve::resolveType(std::shared_ptr<ASTTypeNode> type) {
    // Type definitions can only introduce one level of indirection. So, a newly defined type is
    // either a basic type like int or bool, or a single indirection into the existing types.

    // Resolve the 'to' type for pointers. TODO: test this when language support arrives.
    if (std::shared_ptr<ASTPtrType> ptr_type = std::dynamic_pointer_cast<ASTPtrType>(type)) {
        ptr_type->setToType(resolveType(ptr_type->getToType()));
        return ptr_type;
    }
    // Resolve the element type for arrays. TODO: test this
    else if (std::shared_ptr<ASTArrType> arr_type = std::dynamic_pointer_cast<ASTArrType>(type)) {
        arr_type->setElemType(resolveType(arr_type->getElemType()));
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

bool TypeResolve::visitTypeNode(std::shared_ptr<ASTTypeNode> type) {
    // Just resolve the type in place. We don't need to worry about the return value, because we
    // aren't resolving recursively. That's done by resolveType() itself.
    resolveType(type);
    return ASTVisitor::visitTypeNode(type);
}

bool TypeResolve::visitArgNode(std::shared_ptr<ASTArgNode> argNode) {
    argNode->setType(resolveType(argNode->getType()));
    return ASTVisitor::visitArgNode(argNode);
}

bool TypeResolve::visitFunType(std::shared_ptr<ASTFunType> funType) {
    funType->setReturnType(resolveType(funType->getReturnType()));
    // Arguments are handled in visitArgNode()
    return ASTVisitor::visitFunType(funType);
}

bool TypeResolve::visitVarDeclStmt(std::shared_ptr<ASTVarDeclStmt> varDecl) {
    varDecl->setType(resolveType(varDecl->getType()));
    return ASTVisitor::visitVarDeclStmt(varDecl);
}

bool TypeResolve::visitAllocArrayExp(std::shared_ptr<ASTAllocArrayExp> allocExp) {
    allocExp->setElemType(resolveType(allocExp->getElemType()));
    return ASTVisitor::visitAllocArrayExp(allocExp);
}

bool TypeResolve::visitTypeDecl(std::shared_ptr<ASTTypeDecl> typeDecl) {
    // TypeDecl's define new type names, so we need to update the mapping in the module

    // Types must have a new name
    if (module->getType(typeDecl->getName()) != nullptr)
        throw RedeclaredTypeException(typeDecl->getName());

    // TODO: type cannot be defined in terms of itself
    // TODO: inconsistency between getName() and getId() all over the place

    // Add the new type to the mapping
    module->addType(typeDecl->getName(), resolveType(typeDecl->getType()));

    return ASTVisitor::visitTypeDecl(typeDecl);
}

void TypeResolve::run(std::shared_ptr<ASTDeclSeqNode> ast) {
    visitDeclSeqNode(ast);
}

};
