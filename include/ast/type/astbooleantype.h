/**
 * @file astbooleantype.h
 *
 * @brief Abstract syntax tree integer type
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTBOOLEANTYPE_H
#define __ASTBOOLEANTYPE_H

#include <ast/type/asttype.h>

class ASTBooleanType : public ASTType {
private:

    static ASTBooleanType *instance;

public:

    ASTBooleanType();

    static ASTBooleanType *get();

    bool equal(ASTType *other);

    virtual void print(std::ostream & ss) override;
};

#endif
