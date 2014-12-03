/**
 * @file astassertstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/stmt/astassertstmt.h>

ASTAssertStmt::ASTAssertStmt(std::shared_ptr<ASTExpNode> cond)
    : cond(cond)
{
}

ASTAssertStmt::~ASTAssertStmt() {
}

std::shared_ptr<ASTExpNode> ASTAssertStmt::getCond() {
    return cond;
}

void ASTAssertStmt::print(std::ostream & ss) {
}
