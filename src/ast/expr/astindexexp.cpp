/**
 * @file astindexexp.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astindexexp.h>

ASTIndexExp::ASTIndexExp(std::shared_ptr<ASTExpNode> lvalue, std::shared_ptr<ASTExpNode> subscript)
    : lvalue(lvalue),
      subscript(subscript)
{
}

std::shared_ptr<ASTExpNode> ASTIndexExp::getLValue() {
    return lvalue;
}

std::shared_ptr<ASTExpNode> ASTIndexExp::getSubscript() {
    return subscript;
}

void ASTIndexExp::print(std::ostream & ss) {
    // TODO
}
