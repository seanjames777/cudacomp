/**
 * @file astbinop.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astbinop.h>

ASTBinop::ASTBinop(enum op op, ASTNode *e1, ASTNode *e2)
    : op(op),
      e1(e1),
      e2(e2)
{
}

ASTBinop::~ASTBinop() {
    delete e1;
    delete e2;
}

enum ASTBinop::op ASTBinop::getOp() {
    return op;
}

ASTNode *ASTBinop::getE1() {
    return e1;
}

ASTNode *ASTBinop::getE2() {
    return e2;
}

Value *ASTBinop::codegen(CodegenCtx *ctx) {
    Value *v1 = e1->codegen(ctx);
    Value *v2 = e2->codegen(ctx);

    Instruction::BinaryOps llopt;

    switch(op) {
    case ADD: llopt = Instruction::Add; break;
    case SUB: llopt = Instruction::Sub; break;
    case MUL: llopt = Instruction::Mul; break;
    case DIV: llopt = Instruction::SDiv; break;
    }

    Instruction *instr = BinaryOperator::Create(llopt, v1, v2,
        "", ctx->getBBlock());

    return instr;
}
