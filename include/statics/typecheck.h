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
#include <ast/type/asttypenode.h>
#include <stdexcept>

namespace Statics {

    class UndefinedException : public std::exception {
    private:
        std::string msg;
    public:
        UndefinedException(std::string id);
        virtual const char *what() const _NOEXCEPT override { return msg.c_str(); }
    };

    class UndeclaredException : public std::exception {
    private:
        std::string msg;
    public:
        UndeclaredException(std::string id);
        virtual const char *what() const _NOEXCEPT override { return msg.c_str(); }
    };

    class RedeclaredException : public std::exception {
    private:
        std::string msg;
    public:
        RedeclaredException(std::string id);
        virtual const char *what() const _NOEXCEPT override { return msg.c_str(); }
    };

    class IllegalTypeException : public std::exception {
    private:
        std::string msg;
    public:
        IllegalTypeException();
        virtual const char *what() const _NOEXCEPT override { return msg.c_str(); }
    };

    typedef std::unordered_set<std::string> idset;

    std::shared_ptr<ASTTypeNode> typecheck_exp(
        std::shared_ptr<ModuleInfo> mod,
        std::shared_ptr<FunctionInfo> func,
        idset & decl,
        idset & def,
        std::shared_ptr<ASTExpNode> node);

    void typecheck_stmts(
        std::shared_ptr<ModuleInfo> mod,
        std::shared_ptr<FunctionInfo> func,
        idset & decl,
        idset & def,
        std::shared_ptr<ASTStmtSeqNode> node);

    void typecheck_stmt(
        std::shared_ptr<ModuleInfo> mod,
        std::shared_ptr<FunctionInfo> func,
        idset & decl,
        idset & def,
        std::shared_ptr<ASTStmtNode> node);

    void typecheck_tops(
        std::shared_ptr<ModuleInfo> mod,
        std::shared_ptr<ASTTopSeqNode> node);

    void typecheck_top(
        std::shared_ptr<ModuleInfo> mod,
        std::shared_ptr<ASTTopNode> node);

};

#endif
