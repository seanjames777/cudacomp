/**
 * @file astallocarrayexp.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astallocarrayexp.h>

ASTAllocArrayExp::ASTAllocArrayExp(std::shared_ptr<ASTTypeNode> type, std::shared_ptr<ASTExpNode> length)
    : type(type),
      length(length)
{
}

std::shared_ptr<ASTTypeNode> ASTAllocArrayExp::getElemType() {
    return type;
}

std::shared_ptr<ASTExpNode> ASTAllocArrayExp::getLength() {
    return length;
}

void ASTAllocArrayExp::print(std::ostream & ss) {
    // TODO
}
