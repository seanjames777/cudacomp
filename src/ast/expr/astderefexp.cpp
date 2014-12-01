/**
 * @file astderefexp.cpp
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#include <ast/expr/astderefexp.h>

ASTDerefExp::ASTDerefExp(std::shared_ptr<ASTExpNode> exp)
    : exp(exp)
{
}

ASTDerefExp::~ASTDerefExp() {
}

std::shared_ptr<ASTExpNode> ASTDerefExp::getExp() {
    return exp;
}

void ASTDerefExp::print(std::ostream & ss) {
}
