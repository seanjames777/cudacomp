/**
 * @file astintegerexp.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astintegerexp.h>
#include <iostream>

ASTIntegerExp::ASTIntegerExp(int value)
    : value(value)
{
}

ASTIntegerExp::~ASTIntegerExp() {
}

int ASTIntegerExp::getValue() {
    return value;
}

void ASTIntegerExp::print(std::ostream & ss) {
    ss << value;
}
