/**
 * @file astinteger.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astinteger.h>

ASTInteger::ASTInteger(int value)
    : value(value)
{
}

ASTInteger::~ASTInteger() {
}

int ASTInteger::getValue() {
    return value;
}

void ASTInteger::print(std::ostream & ss) {
    ss << "int(" << value << ")";
}
