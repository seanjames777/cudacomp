/**
 * @file astfundefntop.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/decl/astfundecl.h>

ASTFunDecl::ASTFunDecl(std::string name, std::shared_ptr<ASTFunType> sig,
    bool defn, enum ASTDeclNode::Linkage linkage, std::shared_ptr<ASTStmtSeqNode> body)
    : name(name),
      sig(sig),
      defn(defn),
      linkage(linkage),
      body(body)
{
}

ASTFunDecl::~ASTFunDecl() {
}

std::string ASTFunDecl::getName() {
    return name;
}

bool ASTFunDecl::isDefn() {
    return defn;
}

std::shared_ptr<ASTFunType> ASTFunDecl::getSignature() {
    return sig;
}

std::shared_ptr<ASTStmtSeqNode> ASTFunDecl::getBody() {
    return body;
}

enum ASTDeclNode::Linkage ASTFunDecl::getLinkage() {
    return linkage;
}

void ASTFunDecl::setBody(std::shared_ptr<ASTStmtSeqNode> body) {
    this->body = body;
}

void ASTFunDecl::print(std::ostream & ss) {
    // TODO
}
