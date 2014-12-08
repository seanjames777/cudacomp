/**
 * @file astunopexp.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astunopexp.h>
#include <iostream>

ASTUnopExp::ASTUnopExp(enum op op, std::shared_ptr<ASTExpNode> exp)
    : op(op),
      exp(exp)
{
}

ASTUnopExp::~ASTUnopExp() {
}

enum ASTUnopExp::op ASTUnopExp::getOp() {
    return op;
}

std::shared_ptr<ASTExpNode> ASTUnopExp::getExp() {
    return exp;
}

void ASTUnopExp::print(std::ostream & ss) {
    switch(op) {
    case NOT:
        ss << "!";
        break;
    case BNOT:
        ss << "~";
        break;
    case NEG:
        ss << "-";
        break;
    }
}
