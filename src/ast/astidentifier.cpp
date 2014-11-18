/**
 * @file astidentifier.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astidentifier.h>

ASTIdentifier::ASTIdentifier(char *value)
    : value(value)
{
}

ASTIdentifier::~ASTIdentifier() {
    free(value);
}

char *ASTIdentifier::getValue() {
    return value;
}

Value *ASTIdentifier::codegen(CodegenCtx *ctx) {
    return NULL;
}
