/**
 * @file typeresolve.h
 *
 * @brief Type resolution
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __TYPERESOLVE_H
#define __TYPERESOLVE_H

#include <statics/moduleinfo.h>
#include <statics/functioninfo.h>
#include <ast/ast.h>
#include <ast/astvisitor.h>
#include <statics/exceptions.h>

namespace Statics {

class TypeResolve : public ASTVisitor {
private:

    std::shared_ptr<ModuleInfo> module;

public:

    TypeResolve(std::shared_ptr<ModuleInfo> module);

    std::shared_ptr<ASTTypeNode> resolveType(std::shared_ptr<ASTTypeNode> type);

    virtual void visitTypeNode(std::shared_ptr<ASTTypeNode> type) override;

    virtual void visitArgNode(std::shared_ptr<ASTArgNode> argNode) override;

    virtual void visitFunType(std::shared_ptr<ASTFunType> funType) override;

    virtual void visitVarDeclStmt(std::shared_ptr<ASTVarDeclStmt> varDecl) override;

    virtual void visitRecordDecl(std::shared_ptr<ASTRecordDecl> rcdDecl) override;

    virtual void visitAllocArrayExp(std::shared_ptr<ASTAllocArrayExp> allocExp) override;

    virtual void visitAllocExp(std::shared_ptr<ASTAllocExp> allocExp) override;

    virtual void visitTypeDecl(std::shared_ptr<ASTTypeDecl> typeDecl) override;

    void run(std::shared_ptr<ASTDeclSeqNode> ast);

};

};

#endif
