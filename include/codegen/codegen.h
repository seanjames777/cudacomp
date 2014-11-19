/**
 * @file codegen.h
 *
 * @brief Code generation
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __CODEGEN_H
#define __CODEGEN_H

#include <codegen/codegenctx.h>
#include <ast/expr/astexpnode.h>
#include <ast/stmt/aststmtnode.h>

namespace Codegen {

    Value *codegen_exp(CodegenCtx *ctx, ASTExpNode *node);

    void codegen_stmt(CodegenCtx *ctx, ASTStmtNode *node);

}

#endif
