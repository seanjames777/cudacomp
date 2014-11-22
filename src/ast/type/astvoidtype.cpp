/**
 * @file astvoidtype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astvoidtype.h>

std::shared_ptr<ASTVoidType> ASTVoidType::instance = nullptr;

ASTVoidType::ASTVoidType() {
}

std::shared_ptr<ASTVoidType> ASTVoidType::get() {
    if (!instance)
        instance = std::make_shared<ASTVoidType>();

    return instance;
}

bool ASTVoidType::equal(std::shared_ptr<ASTTypeNode> other) {
    return (other != nullptr) && (std::dynamic_pointer_cast<ASTVoidType>(other) != nullptr);
}

void ASTVoidType::print(std::ostream & ss) {
    ss << "void";
}
