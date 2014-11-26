/**
 * @file astrecorddecl.cpp
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#include <ast/decl/astrecorddecl.h>

ASTRecordDecl::ASTRecordDecl(std::string name, std::shared_ptr<ASTRecordType> sig, bool defn)
    : name(name),
      sig(sig)
{
}

ASTRecordDecl::~ASTRecordDecl() {
}

std::string ASTRecordDecl::getName() {
    return name;
}

std::shared_ptr<ASTRecordType> ASTRecordDecl::getSignature() {
    return sig;
}

void ASTRecordDecl::print(std::ostream & ss) {
    // TODO
}
