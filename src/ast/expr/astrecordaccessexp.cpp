/**
 * @file astrecordaccessexp.cpp
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#include <ast/expr/astrecordaccessexp.h>

ASTRecordAccessExp::ASTRecordAccessExp(std::shared_ptr<ASTExpNode> lvalue, std::string id)
    : lvalue(lvalue),
      id(id)
{
}

std::shared_ptr<ASTExpNode> ASTRecordAccessExp::getLValue() {
    return lvalue;
}

std::string ASTRecordAccessExp::getId() {
    return id;
}

void ASTRecordAccessExp::print(std::ostream & ss) {
    // TODO
}
