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

#include <statics/moduleinfo.h>
#include <statics/functioninfo.h>
#include <ast/ast.h>
#include <statics/exceptions.h>

namespace Statics {

    bool returncheck_stmts(std::shared_ptr<FunctionInfo> func, std::shared_ptr<ASTStmtSeqNode> nodes);

    bool returncheck_stmt(std::shared_ptr<FunctionInfo> func, std::shared_ptr<ASTStmtNode> node);

    void returncheck_tops(std::shared_ptr<ModuleInfo> module, std::shared_ptr<ASTDeclSeqNode> nodes);

    void returncheck_top(std::shared_ptr<ModuleInfo> module, std::shared_ptr<ASTDeclNode> node);

};

#endif
