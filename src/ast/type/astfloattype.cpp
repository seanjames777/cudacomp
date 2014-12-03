/**
 * @file astfloattype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astfloattype.h>

std::shared_ptr<ASTFloatType> ASTFloatType::instance = nullptr;

ASTFloatType::ASTFloatType() {
}

std::shared_ptr<ASTFloatType> ASTFloatType::get() {
    if (!instance)
        instance = std::make_shared<ASTFloatType>();

    return instance;
}

bool ASTFloatType::equal(std::shared_ptr<ASTTypeNode> other) {
    return (other != nullptr) && (std::dynamic_pointer_cast<ASTFloatType>(other) != nullptr);
}

void ASTFloatType::print(std::ostream & ss) {
}
