/**
 * @file typecheck.h
 *
 * @brief Type checking
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __TYPECHECK_H
#define __TYPECHECK_H

#include <statics/typectx.h>
#include <ast/aststmtnode.h>
#include <ast/astexpnode.h>
#include <ast/asttype.h>
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

    ASTType *typecheck_exp(TypeCtx *ctx, idset & decl, idset & def, ASTExpNode *node);

    void typecheck_stmt(TypeCtx *ctx, idset & decl, idset & def, ASTStmtNode *node);

};

#endif
