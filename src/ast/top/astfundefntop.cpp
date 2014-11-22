/**
 * @file astfundefntop.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/top/astfundefntop.h>

ASTFunDefnTop::ASTFunDefnTop(std::string name, std::shared_ptr<ASTFunType> sig, std::shared_ptr<ASTStmtSeqNode> body)
    : name(name),
      sig(sig),
      body(body)
{
}

ASTFunDefnTop::~ASTFunDefnTop() {
}

std::string ASTFunDefnTop::getName() {
    return name;
}

std::shared_ptr<ASTFunType> ASTFunDefnTop::getSignature() {
    return sig;
}

std::shared_ptr<ASTStmtSeqNode> ASTFunDefnTop::getBody() {
    return body;
}

void ASTFunDefnTop::setBody(std::shared_ptr<ASTStmtSeqNode> body) {
    this->body = body;
}

void ASTFunDefnTop::print(std::ostream & ss) {
    // TODO
}
