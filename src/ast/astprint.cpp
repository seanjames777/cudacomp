/**
 * @file astprint.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astprint.h>

ASTPrint::ASTPrint(std::ostream & out, bool colorize)
    : tabDepth(0),
      out(out),
      colorize(colorize)
{
}

void ASTPrint::color(int color) {
    if (!colorize)
        return;

    out << "\033[" << (30 + color) << ";1m";
}

void ASTPrint::uncolor() {
    if (!colorize)
        return;

    out << "\033[0m";
}

void ASTPrint::run(std::shared_ptr<ASTDeclSeqNode> ast) {
    visitDeclSeqNode(ast);
}

void ASTPrint::enter(std::string nodeName, std::shared_ptr<ASTNode> node) {
    for (int i = 0; i < tabDepth; i++)
        out << "  ";

    color(7);
    out << "+ ";

    color(6);
    out << nodeName << " ";
    color(1);
    out << "(";
    color(7);
    node->print(out);
    color(1);
    out << ")";
    uncolor();
    out << std::endl;
    tabDepth++;
}

void ASTPrint::leave() {
    tabDepth--;
}

bool ASTPrint::visitDeclNode(std::shared_ptr<ASTDeclNode> node) {
    ASTVisitor::visitDeclNode(node);
    return true;
}

bool ASTPrint::visitDeclSeqNode(std::shared_ptr<ASTDeclSeqNode> node) {
    ASTVisitor::visitDeclSeqNode(node);
    return true;
}

bool ASTPrint::visitFunDecl(std::shared_ptr<ASTFunDecl> node) {
    enter("ASTFunDecl", node);
    ASTVisitor::visitFunDecl(node);
    leave();

    return true;
}

bool ASTPrint::visitTypeDecl(std::shared_ptr<ASTTypeDecl> node) {
    enter("ASTTypeDecl", node);
    ASTVisitor::visitTypeDecl(node);
    leave();

    return true;
}

bool ASTPrint::visitExpNode(std::shared_ptr<ASTExpNode> node) {
    ASTVisitor::visitExpNode(node);
    return true;
}

bool ASTPrint::visitExpSeqNode(std::shared_ptr<ASTExpSeqNode> node) {
    ASTVisitor::visitExpSeqNode(node);
    return true;
}

bool ASTPrint::visitAllocArrayExp(std::shared_ptr<ASTAllocArrayExp> node) {
    enter("ASTAllocArrayExp", node);
    ASTVisitor::visitAllocArrayExp(node);
    leave();

    return true;
}

bool ASTPrint::visitBinopExp(std::shared_ptr<ASTBinopExp> node) {
    enter("ASTBinopExp", node);
    ASTVisitor::visitBinopExp(node);
    leave();

    return true;
}

bool ASTPrint::visitBooleanExp(std::shared_ptr<ASTBooleanExp> node) {
    enter("ASTBooleanExp", node);
    ASTVisitor::visitBooleanExp(node);
    leave();

    return true;
}

bool ASTPrint::visitCallExp(std::shared_ptr<ASTCallExp> node) {
    enter("ASTCallExp", node);
    ASTVisitor::visitCallExp(node);
    leave();

    return true;
}

bool ASTPrint::visitIdentifierExp(std::shared_ptr<ASTIdentifierExp> node) {
    enter("ASTIdentifierExp", node);
    ASTVisitor::visitIdentifierExp(node);
    leave();

    return true;
}

bool ASTPrint::visitIndexExp(std::shared_ptr<ASTIndexExp> node) {
    enter("ASTIndexExp", node);
    ASTVisitor::visitIndexExp(node);
    leave();

    return true;
}

bool ASTPrint::visitIntegerExp(std::shared_ptr<ASTIntegerExp> node) {
    enter("ASTIntegerExp", node);
    ASTVisitor::visitIntegerExp(node);
    leave();

    return true;
}

bool ASTPrint::visitUnopExp(std::shared_ptr<ASTUnopExp> node) {
    enter("ASTUnopExp", node);
    ASTVisitor::visitUnopExp(node);
    leave();

    return true;
}

bool ASTPrint::visitStmtNode(std::shared_ptr<ASTStmtNode> node) {
    ASTVisitor::visitStmtNode(node);
    return true;
}

bool ASTPrint::visitStmtSeqNode(std::shared_ptr<ASTStmtSeqNode> node) {
    ASTVisitor::visitStmtSeqNode(node);
    return true;
}

bool ASTPrint::visitAssignStmt(std::shared_ptr<ASTAssignStmt> node) {
    enter("ASTAssignStmt", node);
    ASTVisitor::visitAssignStmt(node);
    leave();

    return true;
}

bool ASTPrint::visitExprStmt(std::shared_ptr<ASTExprStmt> node) {
    enter("ASTExprStmt", node);
    ASTVisitor::visitExprStmt(node);
    leave();

    return true;
}

bool ASTPrint::visitIfStmt(std::shared_ptr<ASTIfStmt> node) {
    enter("ASTIfStmt", node);
    ASTVisitor::visitIfStmt(node);
    leave();

    return true;
}

bool ASTPrint::visitReturnStmt(std::shared_ptr<ASTReturnStmt> node) {
    enter("ASTReturnStmt", node);
    ASTVisitor::visitReturnStmt(node);
    leave();

    return true;
}

bool ASTPrint::visitScopeStmt(std::shared_ptr<ASTScopeStmt> node) {
    enter("ASTScopeStmt", node);
    ASTVisitor::visitScopeStmt(node);
    leave();

    return true;
}

bool ASTPrint::visitVarDeclStmt(std::shared_ptr<ASTVarDeclStmt> node) {
    enter("ASTVarDeclStmt", node);
    ASTVisitor::visitVarDeclStmt(node);
    leave();

    return true;
}

bool ASTPrint::visitWhileStmt(std::shared_ptr<ASTWhileStmt> node) {
    enter("ASTWhileStmt", node);
    ASTVisitor::visitWhileStmt(node);
    leave();

    return true;
}

bool ASTPrint::visitArgNode(std::shared_ptr<ASTArgNode> node) {
    enter("ASTArgNode", node);
    ASTVisitor::visitArgNode(node);
    leave();

    return true;
}

bool ASTPrint::visitArgSeqNode(std::shared_ptr<ASTArgSeqNode> node) {
    ASTVisitor::visitArgSeqNode(node);
    return true;
}

bool ASTPrint::visitTypeNode(std::shared_ptr<ASTTypeNode> node) {
    ASTVisitor::visitTypeNode(node);
    return true;
}

bool ASTPrint::visitArrType(std::shared_ptr<ASTArrType> node) {
    enter("ASTArrType", node);
    ASTVisitor::visitArrType(node);
    leave();

    return true;
}

bool ASTPrint::visitBooleanType(std::shared_ptr<ASTBooleanType> node) {
    enter("ASTBooleanType", node);
    ASTVisitor::visitBooleanType(node);
    leave();

    return true;
}

bool ASTPrint::visitFunType(std::shared_ptr<ASTFunType> node) {
    enter("ASTFunType", node);
    ASTVisitor::visitFunType(node);
    leave();

    return true;
}

bool ASTPrint::visitIdType(std::shared_ptr<ASTIdType> node) {
    enter("ASTIdType", node);
    ASTVisitor::visitIdType(node);
    leave();

    return true;
}

bool ASTPrint::visitIntegerType(std::shared_ptr<ASTIntegerType> node) {
    enter("ASTIntegerType", node);
    ASTVisitor::visitIntegerType(node);
    leave();

    return true;
}

bool ASTPrint::visitPtrType(std::shared_ptr<ASTPtrType> node) {
    enter("ASTPtrType", node);
    ASTVisitor::visitPtrType(node);
    leave();

    return true;
}

bool ASTPrint::visitVoidType(std::shared_ptr<ASTVoidType> node) {
    enter("ASTVoidType", node);
    ASTVisitor::visitVoidType(node);
    leave();

    return true;
}
