/**
 * @file astbooleantype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astbooleantype.h>

std::shared_ptr<ASTBooleanType> ASTBooleanType::instance = nullptr;

ASTBooleanType::ASTBooleanType() {
}

std::shared_ptr<ASTBooleanType> ASTBooleanType::get() {
    if (!instance)
        instance = std::make_shared<ASTBooleanType>();

    return instance;
}

bool ASTBooleanType::equal(std::shared_ptr<ASTTypeNode> other) {
    return (other != nullptr) && (std::dynamic_pointer_cast<ASTBooleanType>(other) != nullptr);
}

void ASTBooleanType::print(std::ostream & ss) {
}
