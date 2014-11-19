/**
 * @file astarg.h
 *
 * @brief Abstract syntax function argument node. Combines a type and an argument
 * name. This is not strictly a type, and is also used in function declarations
 * and definitions, but it fits best here as it is especially not an expression
 * or statement.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTARG_H
#define __ASTARG_H

#include <ast/type/asttype.h>
#include <ast/astseqnode.h>

struct ASTArg : public ASTNode {
private:

    ASTType *type;

    // Do not rely on the name, it is only used for parsing function definitions
    std::string name;

public:

    ASTArg(ASTType *type, std::string name);

    ASTType *getType();

    std::string getName();

};

typedef ASTSeqNode<ASTArg> ASTArgSeqNode;

#endif
