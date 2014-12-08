/**
 * @file astcallexp.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astcallexp.h>
#include <iostream>

ASTCallExp::ASTCallExp(
    std::string id,
    std::shared_ptr<ASTExpSeqNode> dim_args,
    std::shared_ptr<ASTExpSeqNode> args)
    : id(id),
      dim_args(dim_args),
      args(args)
{
}

ASTCallExp::~ASTCallExp() {
}

std::string ASTCallExp::getId() {
    return id;
}

void ASTCallExp::setId(std::string id) {
    this->id = id;
}

std::shared_ptr<ASTExpSeqNode> ASTCallExp::getDimArgs() {
    return dim_args;
}

std::shared_ptr<ASTExpSeqNode> ASTCallExp::getArgs() {
    return args;
}

void ASTCallExp::print(std::ostream & ss) {
    ss << id;
}
