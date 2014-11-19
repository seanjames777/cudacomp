/**
 * @file asttype.h
 *
 * @brief Abstract syntax tree abstract type node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTTYPE_H
#define __ASTTYPE_H

#include <ast/astnode.h>

class ASTType : public ASTNode {
public:

    virtual ~ASTType() = 0;

    virtual bool equal(ASTType *other) = 0;

};

#endif
