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
#include <statics/exceptions.h>

namespace Statics {

    typedef std::unordered_set<std::string> idset;

    void funcheck_exp(
        std::shared_ptr<ModuleInfo> mod,
        idset & called,
        std::shared_ptr<ASTExpNode> node);

    void funcheck_stmts(
        std::shared_ptr<ModuleInfo> mod,
        idset & called,
        std::shared_ptr<ASTStmtSeqNode> node);

    void funcheck_stmt(
        std::shared_ptr<ModuleInfo> mod,
        idset & called,
        std::shared_ptr<ASTStmtNode> node);

    void funcheck_tops(
        std::shared_ptr<ModuleInfo> mod,
        std::shared_ptr<ASTDeclSeqNode> node);

    void funcheck_top(
        std::shared_ptr<ModuleInfo> mod,
        idset & called,
        idset & defined,
        std::shared_ptr<ASTDeclNode> node);

};

#endif
