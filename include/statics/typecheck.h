/**
 * @file typecheck.h
 *
 * @brief Type checking
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __TYPECHECK_H
#define __TYPECHECK_H

#include <statics/moduleinfo.h>
#include <statics/functioninfo.h>
#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astexpnode.h>
#include <ast/top/asttopnode.h>
#include <ast/type/asttype.h>
#include <stdexcept>

namespace Statics {

    class UndefinedException : public std::runtime_error {
    public:
        UndefinedException();
    };

    class UndeclaredException : public std::runtime_error {
    public:
        UndeclaredException();
    };

    class RedeclaredException : public std::runtime_error {
    public:
        RedeclaredException();
    };

    class IllegalTypeException : public std::runtime_error {
    public:
        IllegalTypeException();
    };

    typedef std::unordered_set<std::string> idset;

    ASTType *typecheck_exp(ModuleInfo *mod, FunctionInfo *func, idset & decl, idset & def, ASTExpNode *node);

    void typecheck_stmts(ModuleInfo *mod, FunctionInfo *func, idset & decl, idset & def, ASTStmtSeqNode *node);

    void typecheck_stmt(ModuleInfo *mod, FunctionInfo *func, idset & decl, idset & def, ASTStmtNode *node);

    void typecheck_tops(ModuleInfo *mod, ASTTopSeqNode *node);

    void typecheck_top(ModuleInfo *mod, ASTTopNode *node);

};

#endif
