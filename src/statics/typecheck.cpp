/**
 * @file typecheck.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/typecheck.h>
#include <ast/astinteger.h>
#include <ast/astbinop.h>
#include <ast/astseqnode.h>
#include <ast/astreturnstmt.h>

namespace Statics {

Type *typecheck_exp(TypeCtx *ctx, ASTExpNode *node) {
    Type *type = NULL;

    Type *int32 = Type::getInt32Ty(ctx->getContext());

    if (ASTInteger *int_exp = dynamic_cast<ASTInteger *>(node)) {
        type = int32;
    }
    else if (ASTBinop *binop_exp = dynamic_cast<ASTBinop *>(node)) {
        Type *t1 = typecheck_exp(ctx, binop_exp->getE1());
        Type *t2 = typecheck_exp(ctx, binop_exp->getE1());

        switch(binop_exp->getOp()) {
        case ASTBinop::ADD:
        case ASTBinop::SUB:
        case ASTBinop::MUL:
        case ASTBinop::DIV:
            if (t1 != int32 || t2 != int32)
                throw 0;
            type = int32;
        }
    }

    ctx->setType(node, type);

    return type;
}

void typecheck_stmt(TypeCtx *ctx, ASTStmtNode *node) {
    if (ASTSeqNode *seq_node = dynamic_cast<ASTSeqNode *>(node)) {
        typecheck_stmt(ctx, seq_node->getHead());

        if (seq_node->getTail())
            typecheck_stmt(ctx, seq_node->getTail());
    }
    else if (ASTReturnStmt *ret_node = dynamic_cast<ASTReturnStmt *>(node)) {
        Type *expected = Type::getInt32Ty(ctx->getContext());
        Type *exp_type = typecheck_exp(ctx, ret_node->getExp());

        if (exp_type != expected)
            throw 0;
    }
    else {
        throw 0;
    }
}

};
