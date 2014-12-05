/**
 * @file astderefexp.cpp
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#include <ast/expr/astderefexp.h>

ASTDerefExp::ASTDerefExp(std::shared_ptr<ASTExpNode> exp)
    : exp(exp),
      isParenthesized(false)
{
}

ASTDerefExp::~ASTDerefExp() {
}

std::shared_ptr<ASTExpNode> ASTDerefExp::getExp() {
    return exp;
}

bool ASTDerefExp::getParenthesization() {
    return isParenthesized;
}

void ASTDerefExp::setParenthesization(bool parenthesization) {
    isParenthesized = parenthesization;
}

void ASTDerefExp::print(std::ostream & ss) {
}
