/**
 * @file codegen.h
 *
 * @brief Code generator
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
#include <ast/decl/astdeclnode.h>
#include <ast/decl/astfundecl.h>
#include <options.h>

/**
 * @defgroup CodeGen Code Generator
 *
 * @brief The code generator consumes ASTs to produce LLVM IR
 *
 * @{
 */

namespace Codegen {

    Value *codegen_exp(std::shared_ptr<CodegenCtx> ctx, std::shared_ptr<ASTExpNode> node);

    bool codegen_stmts(std::shared_ptr<CodegenCtx> ctx, std::shared_ptr<ASTStmtSeqNode> nodes);

    bool codegen_stmt(std::shared_ptr<CodegenCtx> ctx, std::shared_ptr<ASTStmtNode> node);

    void codegen_top(std::shared_ptr<CodegenCtx> ctx, std::shared_ptr<ASTDeclNode> node);

    void codegen_tops(std::shared_ptr<ModuleInfo> module, std::shared_ptr<ASTDeclSeqNode> nodes, bool emitDevice, std::ostream & out);

}

/**
 * @}
 */

#endif
