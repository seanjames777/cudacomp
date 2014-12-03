/**
 * @file symbolmangle.h
 *
 * @brief Renames functions and calls to functions by prepending the
 * "--symbol-prefix" option.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __SYMBOLMANGLE_H
#define __SYMBOLMANGLE_H

#include <ast/astvisitor.h>
#include <statics/symboltable.h>
#include <options.h>
#include <statics/moduleinfo.h>

// TODO documentation

class SymbolMangle : public ASTVisitor {
private:

    std::shared_ptr<ModuleInfo> module;
    struct CCArgs *args;

    std::string mangleSymbol(std::string name);

public:

    SymbolMangle(std::shared_ptr<ModuleInfo> module);

    void run(std::shared_ptr<ASTDeclSeqNode> ast);

    virtual void visitCallExp(std::shared_ptr<ASTCallExp> call) override;

    virtual void visitFunDecl(std::shared_ptr<ASTFunDecl> funDecl) override;

};

#endif
