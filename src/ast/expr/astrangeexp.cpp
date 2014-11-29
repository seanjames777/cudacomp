/**
 * @file astrangeexp.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astrangeexp.h>

ASTRangeExp::ASTRangeExp(std::shared_ptr<ASTExpNode> min, std::shared_ptr<ASTExpNode> max)
    : min(min),
      max(max)
{
}

ASTRangeExp::~ASTRangeExp() {
}

std::shared_ptr<ASTExpNode> ASTRangeExp::getMin() {
    return min;
}

std::shared_ptr<ASTExpNode> ASTRangeExp::getMax() {
    return max;
}

void ASTRangeExp::print(std::ostream & ss) {
}
