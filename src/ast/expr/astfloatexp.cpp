/**
 * @file astfloatexp.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astfloatexp.h>

ASTFloatExp::ASTFloatExp(float value)
    : value(value)
{
}

ASTFloatExp::~ASTFloatExp() {
}

float ASTFloatExp::getValue() {
    return value;
}

void ASTFloatExp::print(std::ostream & ss) {
    ss << value;
}
