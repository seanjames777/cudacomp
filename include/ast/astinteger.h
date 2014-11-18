/**
 * @file astinteger.h
 *
 * @brief Abstract syntax tree integer node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTINTEGER_H
#define __ASTINTEGER_H

#include <defs.h>
#include <ast/astnode.h>
#include <codegen/codegenctx.h>

class ASTInteger : public ASTNode {
private:

    int value;

public:

    ASTInteger(int value);

    ~ASTInteger();

    int getValue();

    Value *codegen(CodegenCtx *ctx);

};

#endif
