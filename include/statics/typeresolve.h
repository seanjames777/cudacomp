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

    virtual bool visitTypeNode(std::shared_ptr<ASTTypeNode> type) override;

    virtual bool visitArgNode(std::shared_ptr<ASTArgNode> argNode) override;

    virtual bool visitFunType(std::shared_ptr<ASTFunType> funType) override;

    virtual bool visitVarDeclStmt(std::shared_ptr<ASTVarDeclStmt> varDecl) override;

    virtual bool visitAllocArrayExp(std::shared_ptr<ASTAllocArrayExp> allocExp) override;

    virtual bool visitTypeDecl(std::shared_ptr<ASTTypeDecl> typeDecl) override;

    void run(std::shared_ptr<ASTDeclSeqNode> ast);

};

};

#endif
