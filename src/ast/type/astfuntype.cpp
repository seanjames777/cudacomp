/**
 * @file astfuntype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astfuntype.h>

ASTFunType::ASTFunType(std::shared_ptr<ASTTypeNode> returnType, std::shared_ptr<ASTArgSeqNode> args)
    : returnType(returnType),
      args(args)
{
}

std::shared_ptr<ASTTypeNode> ASTFunType::getReturnType() {
    return returnType;
}

void ASTFunType::setReturnType(std::shared_ptr<ASTTypeNode> returnType) {
    this->returnType = returnType;
}

std::shared_ptr<ASTArgSeqNode> ASTFunType::getArgs() {
    return args;
}

bool ASTFunType::equal(std::shared_ptr<ASTTypeNode> other_type) {
    if (other_type == nullptr)
        return false;

    std::shared_ptr<ASTFunType> other = std::dynamic_pointer_cast<ASTFunType>(other_type);

    if (other == nullptr)
        return false;

    if (!returnType->equal(other->returnType))
        return false;

    std::shared_ptr<ASTArgSeqNode> my_arg = args;
    std::shared_ptr<ASTArgSeqNode> other_arg = other->args;

    while (true) {
        if (my_arg == nullptr && other_arg == nullptr)
            return true;
        else if ((my_arg == nullptr && other_arg != nullptr) || (my_arg != nullptr && other_arg == nullptr))
            return false;
        else if (!my_arg->getHead()->getType()->equal(other_arg->getHead()->getType()))
            return false;

        my_arg = my_arg->getTail();
        other_arg = other_arg->getTail();
    }
}

void ASTFunType::print(std::ostream & ss) {
    returnType->print(ss);

    ss << "(";

    std::shared_ptr<ASTArgSeqNode> arg = args;

    while (arg != nullptr) {
        arg->print(ss);

        arg = arg->getTail();

        if (arg != nullptr)
            ss << ", ";
    }

    ss << ")";
}
