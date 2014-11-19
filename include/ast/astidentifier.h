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
#include <ast/astexpnode.h>
#include <codegen/codegenctx.h>

class ASTIdentifier : public ASTExpNode {
private:

    std::string value;

public:

    ASTIdentifier(std::string value);

    ~ASTIdentifier();

    std::string getId();

    void print(std::ostream & ss) override;

};

#endif
