/**
 * @file astbooleanexp.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astbooleanexp.h>

ASTBooleanExp::ASTBooleanExp(bool value)
    : value(value)
{
}

ASTBooleanExp::~ASTBooleanExp() {
}

bool ASTBooleanExp::getValue() {
    return value;
}

void ASTBooleanExp::print(std::ostream & ss) {
    ss << "boolean(" << value << ")";
}
