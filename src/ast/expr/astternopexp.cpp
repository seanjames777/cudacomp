/**
 * @file astternopexp.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astternopexp.h>

ASTTernopExp::ASTTernopExp(
    std::shared_ptr<ASTExpNode> cond,
    std::shared_ptr<ASTExpNode> eTrue,
    std::shared_ptr<ASTExpNode> eFalse)
    : cond(cond),
      eTrue(eTrue),
      eFalse(eFalse),
      type(nullptr)
{
}

ASTTernopExp::~ASTTernopExp() {
}

std::shared_ptr<ASTExpNode> ASTTernopExp::getCond() {
    return cond;
}

std::shared_ptr<ASTExpNode> ASTTernopExp::getTrueExp() {
    return eTrue;
}

std::shared_ptr<ASTExpNode> ASTTernopExp::getFalseExp() {
    return eFalse;
}

std::shared_ptr<ASTTypeNode> ASTTernopExp::getType() {
    return type;
}

void ASTTernopExp::setType(std::shared_ptr<ASTTypeNode> type) {
    this->type = type;
}

void ASTTernopExp::print(std::ostream & ss) {
}
