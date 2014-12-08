/**
 * @file asttypedefntop.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/decl/asttypedecl.h>
#include <iostream>

ASTTypeDecl::ASTTypeDecl(std::string name, std::shared_ptr<ASTTypeNode> type)
    : name(name),
      type(type)
{
}

ASTTypeDecl::~ASTTypeDecl() {
}

std::string ASTTypeDecl::getName() {
    return name;
}

std::shared_ptr<ASTTypeNode> ASTTypeDecl::getType() {
    return type;
}

void ASTTypeDecl::print(std::ostream & ss) {
    ss << name;
}
