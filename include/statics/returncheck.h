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

    bool returncheck_stmts(std::shared_ptr<FunctionInfo> func, std::shared_ptr<ASTStmtSeqNode> nodes);

    bool returncheck_stmt(std::shared_ptr<FunctionInfo> func, std::shared_ptr<ASTStmtNode> node);

    void returncheck_tops(std::shared_ptr<ModuleInfo> module, std::shared_ptr<ASTTopSeqNode> nodes);

    void returncheck_top(std::shared_ptr<ModuleInfo> module, std::shared_ptr<ASTTopNode> node);

};

#endif
