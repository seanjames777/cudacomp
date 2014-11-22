/**
 * @file asttypedefntop.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/top/asttypedefntop.h>

ASTTypeDefnTop::ASTTypeDefnTop(std::string name, std::shared_ptr<ASTTypeNode> type)
    : name(name),
      type(type)
{
}

ASTTypeDefnTop::~ASTTypeDefnTop() {
}

std::string ASTTypeDefnTop::getName() {
    return name;
}

std::shared_ptr<ASTTypeNode> ASTTypeDefnTop::getType() {
    return type;
}

void ASTTypeDefnTop::print(std::ostream & ss) {
    // TODO
}
