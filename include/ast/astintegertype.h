/**
 * @file astintegertype.h
 *
 * @brief Abstract syntax tree integer type
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTINTEGERTYPE_H
#define __ASTINTEGERTYPE_H

#include <ast/asttype.h>

class ASTIntegerType : public ASTType {
private:

    static ASTIntegerType *instance;

public:

    ASTIntegerType();

    static ASTIntegerType *get();

    bool equal(ASTType *other);

    virtual void print(std::ostream & ss) override;

};

#endif
