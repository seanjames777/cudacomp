/**
 * @file returncheck.h
 *
 * @brief Checks that every non-void function has a return statement on every
 * control flow path.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __RETURNCHECK_H
#define __RETURNCHECK_H

#include <ast/stmt/aststmtnode.h>
#include <ast/top/asttopnode.h>
#include <statics/moduleinfo.h>
#include <statics/functioninfo.h>

// TODO: Void functions don't require returns
// TODO: Returns must match function declaration

namespace Statics {

    class NoReturnException : public std::runtime_error {
    public:
        NoReturnException();
    };

    class IllegalReturnTypeException : public std::runtime_error {
    public:
        IllegalReturnTypeException();
    };

    bool returncheck_stmts(FunctionInfo *func, ASTStmtSeqNode *nodes);

    bool returncheck_stmt(FunctionInfo *func, ASTStmtNode *node);

    void returncheck_tops(ModuleInfo *module, ASTTopSeqNode *nodes);

    void returncheck_top(ModuleInfo *module, ASTTopNode *node);

};

#endif
