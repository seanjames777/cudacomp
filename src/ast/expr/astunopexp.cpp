/**
 * @file astunopexp.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astunopexp.h>

ASTUnopExp::ASTUnopExp(enum op op, ASTExpNode *exp)
    : op(op),
      exp(exp)
{
}

ASTUnopExp::~ASTUnopExp() {
    delete exp;
}

enum ASTUnopExp::op ASTUnopExp::getOp() {
    return op;
}

ASTExpNode *ASTUnopExp::getExp() {
    return exp;
}

void ASTUnopExp::print(std::ostream & ss) {
    ss << "un(";
    exp->print(ss);
    ss << ", ";

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

    ss << ")";
}
