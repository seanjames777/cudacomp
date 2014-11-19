/**
 * @file astunop.h
 *
 * @brief Abstract syntax tree unary operation node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTUNOP_H
#define __ASTUNOP_H

#include <defs.h>
#include <ast/expr/astexpnode.h>
#include <codegen/codegenctx.h>

class ASTUnop : public ASTExpNode {
public:

    enum op {
        NOT,
        BNOT,
        NEG
    };

private:

    enum op op;
    ASTExpNode *exp;

public:

    ASTUnop(enum op op, ASTExpNode *exp);

    ~ASTUnop();

    enum op getOp();

    ASTExpNode *getExp();

    void print(std::ostream & ss) override;

};

#endif
