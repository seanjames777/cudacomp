/**
 * @file astbooleantype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astbooleantype.h>

ASTBooleanType *ASTBooleanType::instance = NULL;

ASTBooleanType::ASTBooleanType() {
}

ASTBooleanType *ASTBooleanType::get() {
    if (!instance)
        instance = new ASTBooleanType();

    return instance;
}

bool ASTBooleanType::equal(ASTType *other) {
    return (other != NULL) && (dynamic_cast<ASTBooleanType *>(other) != NULL);
}
