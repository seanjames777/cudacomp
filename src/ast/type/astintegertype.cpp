/**
 * @file astintegertype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astintegertype.h>

std::shared_ptr<ASTIntegerType> ASTIntegerType::instance = nullptr;

ASTIntegerType::ASTIntegerType() {
}

std::shared_ptr<ASTIntegerType> ASTIntegerType::get() {
    if (!instance)
        instance = std::make_shared<ASTIntegerType>();

    return instance;
}

bool ASTIntegerType::equal(std::shared_ptr<ASTTypeNode> other) {
    return (other != nullptr) && (std::dynamic_pointer_cast<ASTIntegerType>(other) != nullptr);
}

void ASTIntegerType::print(std::ostream & ss) {
    ss << "int";
}
