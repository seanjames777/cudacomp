/**
 * @file astexpnode.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astexpnode.h>

ASTExpNode::ASTExpNode()
    : type(nullptr)
{
}

ASTExpNode::~ASTExpNode() {
}

std::shared_ptr<ASTTypeNode> ASTExpNode::getType() {
    return type;
}

void ASTExpNode::setType(std::shared_ptr<ASTTypeNode> type) {
    this->type = type;
}
