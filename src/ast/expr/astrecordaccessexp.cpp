/**
 * @file astrecordaccessexp.cpp
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#include <ast/expr/astrecordaccessexp.h>

ASTRecordAccessExp::ASTRecordAccessExp(std::shared_ptr<ASTExpNode> lvalue, std::string id)
    : lvalue(lvalue),
      id(id),
      type(nullptr)
{
}

std::shared_ptr<ASTExpNode> ASTRecordAccessExp::getLValue() {
    return lvalue;
}

std::string ASTRecordAccessExp::getId() {
    return id;
}

std::shared_ptr<ASTRecordType> ASTRecordAccessExp::getType() {
    return type;
}

void ASTRecordAccessExp::setType(std::shared_ptr<ASTRecordType> t) {
    type = t;
}

void ASTRecordAccessExp::print(std::ostream & ss) {
    lvalue->print(ss);

    ss << ".";
    ss << id;
}
