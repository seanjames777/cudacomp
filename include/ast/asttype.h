/**
 * @file asttype.h
 *
 * @brief Abstract syntax tree abstract type node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTTYPE_H
#define __ASTTYPE_H

#include <defs.h>

class ASTType {
public:

    virtual ~ASTType() = 0;

    virtual bool equal(ASTType *other) = 0;

    virtual void print(std::ostream & ss) = 0;

};

#endif
