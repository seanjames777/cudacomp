/**
 * @file astidentifierexp.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astidentifierexp.h>

ASTIdentifierExp::ASTIdentifierExp(std::string value)
    : value(value),
      lvalue(false)
{
}

ASTIdentifierExp::~ASTIdentifierExp() {
}

std::string ASTIdentifierExp::getId() {
    return value;
}

void ASTIdentifierExp::setId(std::string id) {
    this->value = id;
}

bool ASTIdentifierExp::isLValue() {
    return lvalue;
}

void ASTIdentifierExp::setIsLValue(bool lvalue) {
    this->lvalue = lvalue;
}

void ASTIdentifierExp::print(std::ostream & ss) {
    ss << value;

    if (lvalue)
        ss << ", lvalue";
}
