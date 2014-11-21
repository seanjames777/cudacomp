/**
 * @file astidentifierexp.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astidentifierexp.h>

ASTIdentifierExp::ASTIdentifierExp(std::string value)
    : value(value)
{
}

ASTIdentifierExp::~ASTIdentifierExp() {
}

std::string ASTIdentifierExp::getId() {
    return value;
}

void ASTIdentifierExp::print(std::ostream & ss) {
    ss << "id(" << value << ")";
}
