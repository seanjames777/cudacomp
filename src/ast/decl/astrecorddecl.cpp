/**
 * @file astrecorddecl.cpp
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#include <ast/decl/astrecorddecl.h>

ASTRecordDecl::ASTRecordDecl(std::string name, std::shared_ptr<ASTRecordType> sig, bool defn)
    : name(name),
      sig(sig),
      defn(defn)
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

bool ASTRecordDecl::isDefn() {
    return defn;
}

void ASTRecordDecl::print(std::ostream & ss) {
    ss << "struct ";

    ss << name;

    ss << "{";

    std::shared_ptr<ASTArgSeqNode> field = sig->getFields();

    while (field != nullptr) {
        field->print(ss);

        field = field->getTail();

        if (field != nullptr)
            ss << ", ";
    }

    ss << "}";
}
