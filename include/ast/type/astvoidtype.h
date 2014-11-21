/**
 * @file astvoidtype.h
 *
 * @brief Abstract syntax tree void type
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTVOIDTYPE_H
#define __ASTVOIDTYPE_H

#include <ast/type/asttype.h>

class ASTVoidType : public ASTType {
private:

    static ASTVoidType *instance;

public:

    ASTVoidType();

    static ASTVoidType *get();

    bool equal(ASTType *other);

    virtual void print(std::ostream & ss) override;

};

#endif
