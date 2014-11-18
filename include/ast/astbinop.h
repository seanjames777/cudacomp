/**
 * @file astbinop.h
 *
 * @brief Abstract syntax tree binary operation node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTBINOP_H
#define __ASTBINOP_H

#include <defs.h>
#include <ast/astexpnode.h>
#include <codegen/codegenctx.h>

class ASTBinop : public ASTExpNode {
public:

    enum op {
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        SHL,
        SHR,
        AND,
        OR,
        BAND,
        BOR,
        BXOR
    };

    // TODO: Negative, Not, BitNot

private:

    enum op op;
    ASTExpNode *e1;
    ASTExpNode *e2;

public:

    ASTBinop(enum op op, ASTExpNode *e1, ASTExpNode *e2);

    ~ASTBinop();

    enum op getOp();

    ASTExpNode *getE1();

    ASTExpNode *getE2();

};

#endif
