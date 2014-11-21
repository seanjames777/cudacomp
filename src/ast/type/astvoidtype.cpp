/**
 * @file astvoidtype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astvoidtype.h>

ASTVoidType *ASTVoidType::instance = NULL;

ASTVoidType::ASTVoidType() {
}

ASTVoidType *ASTVoidType::get() {
    if (!instance)
        instance = new ASTVoidType();

    return instance;
}

bool ASTVoidType::equal(ASTType *other) {
    return (other != NULL) && (dynamic_cast<ASTVoidType *>(other) != NULL);
}

void ASTVoidType::print(std::ostream & ss) {
    ss << "void";
}
