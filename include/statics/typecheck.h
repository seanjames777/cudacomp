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

    ASTTypeNode *typecheck_exp(ModuleInfo *mod, FunctionInfo *func, idset & decl, idset & def, ASTExpNode *node);

    void typecheck_stmts(ModuleInfo *mod, FunctionInfo *func, idset & decl, idset & def, ASTStmtSeqNode *node);

    void typecheck_stmt(ModuleInfo *mod, FunctionInfo *func, idset & decl, idset & def, ASTStmtNode *node);

    void typecheck_tops(ModuleInfo *mod, ASTTopSeqNode *node);

    void typecheck_top(ModuleInfo *mod, ASTTopNode *node);

};

#endif
