/**
 * @file astfuntype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astfuntype.h>

ASTFunType::ASTFunType(ASTType *returnType, ASTArgSeqNode *args)
    : returnType(returnType),
      args(args)
{
}

ASTType *ASTFunType::getReturnType() {
    return returnType;
}

ASTArgSeqNode *ASTFunType::getArgs() {
    return args;
}

bool ASTFunType::equal(ASTType *other_type) {
    if (other_type == NULL)
        return false;

    ASTFunType *other = dynamic_cast<ASTFunType *>(other_type);

    if (other == NULL)
        return false;

    if (!returnType->equal(other->returnType))
        return false;

    ASTArgSeqNode *my_arg = args;
    ASTArgSeqNode *other_arg = other->args;

    while (true) {
        if (my_arg == NULL && other_arg == NULL)
            return true;
        else if ((my_arg == NULL && other_arg != NULL) || (my_arg != NULL && other_arg == NULL))
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

    ASTArgSeqNode *arg = args;

    while (arg != NULL) {
        arg->print(ss);

        arg = arg->getTail();

        if (arg != NULL)
            ss << ", ";
    }

    ss << ")";
}
