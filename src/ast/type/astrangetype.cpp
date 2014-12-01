/**
 * @file astrangetype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astrangetype.h>

std::shared_ptr<ASTRangeType> ASTRangeType::instance = nullptr;

ASTRangeType::ASTRangeType() {
}

std::shared_ptr<ASTRangeType> ASTRangeType::get() {
    if (!instance)
        instance = std::make_shared<ASTRangeType>();

    return instance;
}

bool ASTRangeType::equal(std::shared_ptr<ASTTypeNode> other) {
    return (other != nullptr) && (std::dynamic_pointer_cast<ASTRangeType>(other) != nullptr);
}

void ASTRangeType::print(std::ostream & ss) {
}
