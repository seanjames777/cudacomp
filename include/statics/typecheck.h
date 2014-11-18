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

namespace Statics {

    Type *typecheck_exp(TypeCtx *ctx, ASTExpNode *node);

    void typecheck_stmt(TypeCtx *ctx, ASTStmtNode *node);

};

#endif
