/**
 * @file astfuntype.h
 *
 * @brief Abstract syntax tree function type node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTFUNTYPE_H
#define __ASTFUNTYPE_H

#include <ast/type/asttype.h>
#include <ast/type/astarg.h>

class ASTFunType : public ASTType {
private:

    ASTType *returnType;
    ASTArgSeqNode *args;

public:

    ASTFunType(ASTType *returnType, ASTArgSeqNode *args);

    ASTType *getReturnType();

    ASTArgSeqNode *getArgs();

    bool equal(ASTType *other);

    void print(std::ostream & ss);

};

#endif
