/**
 * @file astarg.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astarg.h>

ASTArg::ASTArg(ASTTypeNode *type, std::string name)
    : type(type),
      name(name)
{
}

ASTTypeNode *ASTArg::getType() {
    return type;
}

std::string ASTArg::getName() {
    return name;
}

void ASTArg::print(std::ostream & ss) {
    ss << "param(";
    type->print(ss);
    ss << ", " << name << ")";
}
