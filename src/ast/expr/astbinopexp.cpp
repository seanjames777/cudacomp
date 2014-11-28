/**
 * @file astbinopexp.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astbinopexp.h>

ASTBinopExp::ASTBinopExp(enum op op, std::shared_ptr<ASTExpNode> e1, std::shared_ptr<ASTExpNode> e2)
    : op(op),
      e1(e1),
      e2(e2),
      type(nullptr)
{
}

ASTBinopExp::~ASTBinopExp() {
}

enum ASTBinopExp::op ASTBinopExp::getOp() {
    return op;
}

std::shared_ptr<ASTExpNode> ASTBinopExp::getE1() {
    return e1;
}

std::shared_ptr<ASTExpNode> ASTBinopExp::getE2() {
    return e2;
}

std::shared_ptr<ASTTypeNode> ASTBinopExp::getType() {
    return type;
}

void ASTBinopExp::setType(std::shared_ptr<ASTTypeNode> type) {
    this->type = type;
}

void ASTBinopExp::print(std::ostream & ss) {
    switch(op) {
    case ADD:
        ss << "+";
        break;
    case SUB:
        ss << "-";
        break;
    case MUL:
        ss << "*";
        break;
    case DIV:
        ss << "/";
        break;
    case MOD:
        ss << "%";
        break;
    case SHL:
        ss << "<<";
        break;
    case SHR:
        ss << ">>";
        break;
    case AND:
        ss << "&&";
        break;
    case OR:
        ss << "||";
        break;
    case BAND:
        ss << "&";
        break;
    case BOR:
        ss << "|";
        break;
    case BXOR:
        ss << "^";
        break;
    case LT:
        ss << "<";
        break;
    case GT:
        ss << ">";
        break;
    case LEQ:
        ss << "<=";
        break;
    case GEQ:
        ss << ">=";
        break;
    case EQ:
        ss << "==";
        break;
    case NEQ:
        ss << "!=";
        break;
    }

    // TODO: If the AST was printed after all the statics, then we would know the
    // types, lvalues, etc.
}
