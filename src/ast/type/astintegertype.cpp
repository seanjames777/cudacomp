/**
 * @file astintegertype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astintegertype.h>

ASTIntegerType *ASTIntegerType::instance = NULL;

ASTIntegerType::ASTIntegerType() {
}

ASTIntegerType *ASTIntegerType::get() {
    if (!instance)
        instance = new ASTIntegerType();

    return instance;
}

bool ASTIntegerType::equal(ASTTypeNode *other) {
    return (other != NULL) && (dynamic_cast<ASTIntegerType *>(other) != NULL);
}

void ASTIntegerType::print(std::ostream & ss) {
    ss << "int";
}
