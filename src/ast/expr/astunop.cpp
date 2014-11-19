/**
 * @file astunop.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astunop.h>

ASTUnop::ASTUnop(enum op op, ASTExpNode *exp)
    : op(op),
      exp(exp)
{
}

ASTUnop::~ASTUnop() {
    delete exp;
}

enum ASTUnop::op ASTUnop::getOp() {
    return op;
}

ASTExpNode *ASTUnop::getExp() {
    return exp;
}

void ASTUnop::print(std::ostream & ss) {
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
