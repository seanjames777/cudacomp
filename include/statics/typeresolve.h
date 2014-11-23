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
#include <statics/exceptions.h>

namespace Statics {

    void typeresolve_stmts(
        std::shared_ptr<ModuleInfo> mod,
        std::shared_ptr<ASTStmtSeqNode> node);

    void typeresolve_stmt(
        std::shared_ptr<ModuleInfo> mod,
        std::shared_ptr<ASTStmtNode> node);

    void typeresolve_tops(
        std::shared_ptr<ModuleInfo> mod,
        std::shared_ptr<ASTDeclSeqNode> node);

    void typeresolve_top(
        std::shared_ptr<ModuleInfo> mod,
        std::shared_ptr<ASTDeclNode> node);

};

#endif