/**
 * @file astptrtype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astptrtype.h>

ASTPtrType::ASTPtrType(ASTTypeNode *toType)
    : toType(toType)
{
}

ASTTypeNode *ASTPtrType::getToType() {
    return toType;
}

bool ASTPtrType::equal(ASTTypeNode *other) {
    if (!other)
        return false;

    if (ASTPtrType *other_ptr = dynamic_cast<ASTPtrType *>(other))
        return toType->equal(other_ptr->getToType());

    return false;
}

void ASTPtrType::print(std::ostream & ss) {
    // TODO
}
