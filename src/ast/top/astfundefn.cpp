/**
 * @file astfundefn.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/top/astfundefn.h>

ASTFunDefn::ASTFunDefn(std::string name, ASTFunType *sig, ASTStmtSeqNode *body)
    : name(name),
      sig(sig),
      body(body)
{
}

ASTFunDefn::~ASTFunDefn() {
    if (sig)
        delete sig;

    if (body)
        delete body;
}

std::string ASTFunDefn::getName() {
    return name;
}

ASTFunType *ASTFunDefn::getSignature() {
    return sig;
}

ASTStmtSeqNode *ASTFunDefn::getBody() {
    return body;
}

void ASTFunDefn::setBody(ASTStmtSeqNode *body) {
    this->body = body;
}

void ASTFunDefn::print(std::ostream & ss) {
    // TODO
}
