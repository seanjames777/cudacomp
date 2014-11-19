/**
 * @file astidentifier.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astidentifier.h>

ASTIdentifier::ASTIdentifier(std::string value)
    : value(value)
{
}

ASTIdentifier::~ASTIdentifier() {
}

std::string ASTIdentifier::getId() {
    return value;
}
