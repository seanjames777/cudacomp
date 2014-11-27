/**
 * @file astrangeforstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/stmt/astrangeforstmt.h>

ASTRangeForStmt::ASTRangeForStmt(
    std::shared_ptr<ASTTypeNode> type,
    std::string id,
    std::shared_ptr<ASTExpNode> range,
    std::shared_ptr<ASTStmtSeqNode> body)
    : type(type),
      id(id),
      range(range),
      body(body)
{
}

ASTRangeForStmt::~ASTRangeForStmt() {
}

std::shared_ptr<ASTTypeNode> ASTRangeForStmt::getIteratorType() {
    return type;
}

std::string ASTRangeForStmt::getIteratorID() {
    return id;
}

void ASTRangeForStmt::setIteratorID(std::string id) {
    this->id = id;
}

std::shared_ptr<ASTExpNode> ASTRangeForStmt::getRange() {
    return range;
}

std::shared_ptr<ASTStmtSeqNode> ASTRangeForStmt::getBody() {
    return body;
}

void ASTRangeForStmt::print(std::ostream & ss) {
    ss << id;
}
