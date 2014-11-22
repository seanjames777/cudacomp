/**
 * @file astcallexp.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astcallexp.h>

ASTCallExp::ASTCallExp(std::string id, std::shared_ptr<ASTExpSeqNode> args)
    : id(id),
      args(args)
{
}

ASTCallExp::~ASTCallExp() {
}

std::string ASTCallExp::getId() {
    return id;
}

std::shared_ptr<ASTExpSeqNode> ASTCallExp::getArgs() {
    return args;
}

void ASTCallExp::print(std::ostream & ss) {
    // TODO
}
