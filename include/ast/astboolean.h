/**
 * @file astboolean.h
 *
 * @brief Abstract syntax tree integer node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTBOOLEAN_H
#define __ASTBOOLEAN_H

#include <defs.h>
#include <ast/astexpnode.h>
#include <codegen/codegenctx.h>

class ASTBoolean : public ASTExpNode {
private:

    bool value;

public:

    ASTBoolean(bool value);

    ~ASTBoolean();

    bool getValue();

};

#endif
