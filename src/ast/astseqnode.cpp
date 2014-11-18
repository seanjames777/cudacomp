/**
 * @file astseqnode.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astseqnode.h>

ASTSeqNode::ASTSeqNode(ASTStmtNode *head, ASTSeqNode *tail)
    : head(head),
      tail(tail)
{
}

ASTSeqNode::~ASTSeqNode() {
    delete head;

    if (tail)
        delete tail;
}

void ASTSeqNode::codegen(CodegenCtx *ctx) {
    head->codegen(ctx);

    if (tail)
        tail->codegen(ctx);
}

ASTStmtNode *ASTSeqNode::getHead() {
    return head;
}

ASTSeqNode *ASTSeqNode::getTail() {
    return tail;
}
