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
#include <ast/astnode.h>
#include <codegen/codegenctx.h>

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

    ASTBinop(enum op op, ASTNode *e1, ASTNode *e2);

    ~ASTBinop();

    enum op getOp();

    ASTNode *getE1();

    ASTNode *getE2();

    Value *codegen(CodegenCtx *ctx);

};

#endif
