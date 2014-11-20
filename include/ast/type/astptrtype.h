/**
 * @file astptrtype.h
 *
 * @brief Abstract syntax tree pointer type
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTPTRTYPE_H
#define __ASTPTRTYPE_H

#include <ast/type/asttype.h>

class ASTPtrType : public ASTType {
private:

    ASTType *toType;

public:

    ASTPtrType(ASTType *toType);

    ASTType *getToType();

    bool equal(ASTType *other);

    virtual void print(std::ostream & ss) override;

};

#endif
