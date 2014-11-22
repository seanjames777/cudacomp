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
#include <ast/top/astfundefntop.h>

namespace Codegen {

    Value *codegen_exp(CodegenCtx *ctx, std::shared_ptr<ASTExpNode> node);

    bool codegen_stmts(CodegenCtx *ctx, std::shared_ptr<ASTStmtSeqNode> nodes);

    bool codegen_stmt(CodegenCtx *ctx, std::shared_ptr<ASTStmtNode> node);

    void codegen_top(CodegenCtx *ctx, std::shared_ptr<ASTTopNode> node);

    void codegen_tops(std::shared_ptr<ModuleInfo> module, std::shared_ptr<ASTTopSeqNode> nodes, bool emitDevice, std::ostream & out);

}

#endif
