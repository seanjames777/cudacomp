/**
 * @file funcheck.h
 *
 * @brief Function checking
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __FUNCHECK_H
#define __FUNCHECK_H

#include <statics/moduleinfo.h>
#include <statics/functioninfo.h>
#include <ast/ast.h>
#include <ast/astvisitor.h>
#include <statics/exceptions.h>

namespace Statics {

/**
 * @brief Abstract syntax tree analysis which checks the rules below.
 *
 * - Functions may be declared more than once
 * - All declarations of a function must have the same signature and linkage
 * - Internal functions may only be defined once
 * - External functions may not be defined
 * - Functions must be declared before they can be called
 * - Any internal function that is called must be defined
 */
class FunCheck : public ASTVisitor {
private:

    typedef std::unordered_set<std::string> idset;

    std::shared_ptr<ModuleInfo> module;
    idset called;
    idset defined;

public:

    FunCheck(std::shared_ptr<ModuleInfo> module);

    void run(std::shared_ptr<ASTDeclSeqNode> ast);

    void visitCallExp(std::shared_ptr<ASTCallExp> call_exp) override;

    void visitFunDecl(std::shared_ptr<ASTFunDecl> funDefn) override;

};

}

#endif
