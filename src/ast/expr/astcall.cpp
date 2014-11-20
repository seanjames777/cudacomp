/**
 * @file astcall.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astcall.h>

ASTCall::ASTCall(std::string id, ASTExpSeqNode *args)
    : id(id),
      args(args)
{
}

ASTCall::~ASTCall() {
    delete args;
}

std::string ASTCall::getId() {
    return id;
}

ASTExpSeqNode *ASTCall::getArgs() {
    return args;
}

void ASTCall::print(std::ostream & ss) {
    // TODO
}
