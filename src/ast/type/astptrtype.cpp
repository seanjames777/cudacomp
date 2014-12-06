/**
 * @file astptrtype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astptrtype.h>

static std::shared_ptr<ASTPtrType> null_ptr_type = nullptr;

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

    if (std::shared_ptr<ASTPtrType> other_ptr = std::dynamic_pointer_cast<ASTPtrType>(other)) {
        if (toType == nullptr && other_ptr->getToType() == nullptr)
            return true;
        else if (toType == nullptr || other_ptr->getToType() == nullptr)
            return false;
        else
            return toType->equal(other_ptr->getToType());
    }

    return false;
}

void ASTPtrType::print(std::ostream & ss) {
}

const std::shared_ptr<ASTPtrType> ASTPtrType::getNullPtr() {
    if (!null_ptr_type)
        null_ptr_type = std::make_shared<ASTPtrType>(nullptr);

    return null_ptr_type;
}
