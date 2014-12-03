/**
 * @file astarrtype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astarrtype.h>

ASTArrType::ASTArrType(std::shared_ptr<ASTTypeNode> elemType)
    : elemType(elemType)
{
}

std::shared_ptr<ASTTypeNode> ASTArrType::getElemType() {
    return elemType;
}

void ASTArrType::setElemType(std::shared_ptr<ASTTypeNode> elemType) {
    this->elemType = elemType;
}

bool ASTArrType::equal(std::shared_ptr<ASTTypeNode> other) {
    if (!other)
        return false;

    if (std::shared_ptr<ASTArrType> other_ptr = std::dynamic_pointer_cast<ASTArrType>(other))
        return elemType->equal(other_ptr->getElemType());

    return false;
}

void ASTArrType::print(std::ostream & ss) {
}
