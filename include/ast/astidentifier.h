/**
 * @file astidentifier.h
 *
 * @brief Abstract syntax tree identifier node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTIDENTIFIER_H
#define __ASTIDENTIFIER_H

#include <defs.h>
#include <ast/astnode.h>
#include <codegen/codegenctx.h>

class ASTIdentifier : public ASTNode {
private:

    char *value;

public:

    ASTIdentifier(char *value);

    ~ASTIdentifier();

    char *getValue();

    Value *codegen(CodegenCtx *ctx);

};

#endif