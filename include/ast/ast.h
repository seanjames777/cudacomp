/**
 * @file ast.h
 *
 * @brief Abstract syntax tree
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __AST_H
#define __AST_H

#include <defs.h>
#include <codegen/codegenctx.h>

class CodegenCtx;

class ASTNode {
public:

    virtual ~ASTNode() {
    }

    virtual Value *codegen(CodegenCtx *ctx) {
        return NULL;
    }
};

class ASTInteger : public ASTNode {
private:

    int value;

public:

    ASTInteger(int value)
        : value(value)
    {
    }

    ~ASTInteger() {
    }

    int getValue() {
        return value;
    }

    Value *codegen(CodegenCtx *ctx) {
        Type *type = Type::getInt32Ty(ctx->getContext());
        return ConstantInt::get(type, value);
    }

};

class ASTIdentifier : public ASTNode {
private:

    char *value;

public:

    ASTIdentifier(char *value)
        : value(value)
    {
    }

    ~ASTIdentifier() {
        free(value);
    }

    char *getValue() {
        return value;
    }

};

class ASTBinop : public ASTNode {
public:

    enum op {
        ADD,
        SUB,
        MUL,
        DIV
    };

private:

    enum op op;
    ASTNode *e1;
    ASTNode *e2;

public:

    ASTBinop(enum op op, ASTNode *e1, ASTNode *e2)
        : op(op),
          e1(e1),
          e2(e2)
    {
    }

    ~ASTBinop() {
        delete e1;
        delete e2;
    }

    enum op getOp() {
        return op;
    }

    ASTNode *getE1() {
        return e1;
    }

    ASTNode *getE2() {
        return e2;
    }

    Value *codegen(CodegenCtx *ctx) {
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

};

#endif
