/**
 * @file astfundefntop.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/top/astfundefntop.h>

ASTFunDefnTop::ASTFunDefnTop(std::string name, ASTFunType *sig, ASTStmtSeqNode *body)
    : name(name),
      sig(sig),
      body(body)
{
}

ASTFunDefnTop::~ASTFunDefnTop() {
    if (sig)
        delete sig;

    if (body)
        delete body;
}

std::string ASTFunDefnTop::getName() {
    return name;
}

ASTFunType *ASTFunDefnTop::getSignature() {
    return sig;
}

ASTStmtSeqNode *ASTFunDefnTop::getBody() {
    return body;
}

void ASTFunDefnTop::setBody(ASTStmtSeqNode *body) {
    this->body = body;
}

void ASTFunDefnTop::print(std::ostream & ss) {
    // TODO
}
