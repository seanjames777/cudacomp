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
#include <ast/ast.h>
#include <statics/exceptions.h>

namespace Statics {

    typedef std::set<std::string> idset;

    std::shared_ptr<ASTTypeNode> typecheck_exp(
        std::shared_ptr<ModuleInfo> mod,
        std::shared_ptr<FunctionInfo> func,
        std::shared_ptr<ASTExpNode> node);

    void typecheck_stmts(
        std::shared_ptr<ModuleInfo> mod,
        std::shared_ptr<FunctionInfo> func,
        std::shared_ptr<ASTStmtSeqNode> node);

    void typecheck_stmt(
        std::shared_ptr<ModuleInfo> mod,
        std::shared_ptr<FunctionInfo> func,
        std::shared_ptr<ASTStmtNode> node);

    void typecheck_tops(
        std::shared_ptr<ModuleInfo> mod,
        std::shared_ptr<ASTDeclSeqNode> node);

    void typecheck_top(
        std::shared_ptr<ModuleInfo> mod,
        std::shared_ptr<ASTDeclNode> node);

};

#endif
