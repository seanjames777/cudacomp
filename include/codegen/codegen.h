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
#include <statics/moduleinfo.h>
#include <statics/functioninfo.h>
#include <ast/top/asttopnode.h>
#include <ast/top/astfundefn.h>

namespace Codegen {

    Value *codegen_exp(CodegenCtx *ctx, ASTExpNode *node);

    void codegen_stmts(CodegenCtx *ctx, ASTStmtSeqNode *nodes);

    bool codegen_stmt(CodegenCtx *ctx, ASTStmtNode *node);

    void codegen_top(CodegenCtx *ctx, ASTTopNode *node);

    void codegen_tops(ModuleInfo *module, ASTTopSeqNode *nodes, bool emitDevice, std::ostream & out);

}

#endif
