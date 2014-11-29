/**
 * @file astargnode.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astargnode.h>

ASTArgNode::ASTArgNode(std::shared_ptr<ASTTypeNode> type, std::string name)
    : type(type),
      name(name)
{
}

std::shared_ptr<ASTTypeNode> ASTArgNode::getType() {
    return type;
}

void ASTArgNode::setType(std::shared_ptr<ASTTypeNode> type) {
    this->type = type;
}

std::string ASTArgNode::getName() {
    return name;
}

void ASTArgNode::setName(std::string name) {
    this->name = name;
}

void ASTArgNode::print(std::ostream & ss) {
    ss << name;
}
