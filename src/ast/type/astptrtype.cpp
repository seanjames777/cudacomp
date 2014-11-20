/**
 * @file astptrtype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astptrtype.h>

ASTPtrType::ASTPtrType(ASTType *toType)
    : toType(toType)
{
}

ASTType *getToType() {
    return toType;
}

bool ASTPtrType::equal(ASTType *other) {
    if (other && (ASTPtrType *other_ptr = dynamic_cast<ASTPtrType *>(other)))
        return toType->equal(other_ptr->getToType());

    return false;
}

void ASTPtrType::print(std::ostream & ss) {
    // TODO
}
