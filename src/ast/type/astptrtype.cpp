/**
 * @file astptrtype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astptrtype.h>

ASTPtrType::ASTPtrType(std::shared_ptr<ASTTypeNode> toType)
    : toType(toType)
{
}

std::shared_ptr<ASTTypeNode> ASTPtrType::getToType() {
    return toType;
}

void ASTPtrType::setToType(std::shared_ptr<ASTTypeNode> toType) {
    this->toType = toType;
}

bool ASTPtrType::equal(std::shared_ptr<ASTTypeNode> other) {
    if (!other)
        return false;

    if (std::shared_ptr<ASTPtrType> other_ptr = std::dynamic_pointer_cast<ASTPtrType>(other))
        return toType->equal(other_ptr->getToType());

    return false;
}

void ASTPtrType::print(std::ostream & ss) {
}

int ASTPtrType::getSize() {
    return 8;
}
