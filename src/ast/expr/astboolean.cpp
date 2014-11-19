/**
 * @file astboolean.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astboolean.h>

ASTBoolean::ASTBoolean(bool value)
    : value(value)
{
}

ASTBoolean::~ASTBoolean() {
}

bool ASTBoolean::getValue() {
    return value;
}

void ASTBoolean::print(std::ostream & ss) {
    ss << "boolean(" << value << ")";
}
