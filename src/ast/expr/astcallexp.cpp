/**
 * @file astcallexp.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astcallexp.h>

ASTCallExp::ASTCallExp(std::string id, ASTExpSeqNode *args)
    : id(id),
      args(args)
{
}

ASTCallExp::~ASTCallExp() {
    delete args;
}

std::string ASTCallExp::getId() {
    return id;
}

ASTExpSeqNode *ASTCallExp::getArgs() {
    return args;
}

void ASTCallExp::print(std::ostream & ss) {
    // TODO
}
