/**
 * @file astprint.h
 *
 * @brief AST printing utility
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTPRINT_H
#define __ASTPRINT_H

#include <ast/ast.h>
#include <ast/astvisitor.h>

/**
 * @brief AST printing utility
 */
class ASTPrint : public ASTVisitor {
private:

    int tabDepth;
    std::ostream & out;
    bool colorize;

    void enter(std::string nodeName, std::shared_ptr<ASTNode> node);

    void leave();

    void color(int color);

    void uncolor();

public:

    ASTPrint(std::ostream & out, bool colorize);

    void run(std::shared_ptr<ASTDeclSeqNode> ast);

    virtual bool visitDeclNode(std::shared_ptr<ASTDeclNode> node) override;

    virtual bool visitDeclSeqNode(std::shared_ptr<ASTDeclSeqNode> node) override;

    virtual bool visitFunDecl(std::shared_ptr<ASTFunDecl> node) override;

    virtual bool visitTypeDecl(std::shared_ptr<ASTTypeDecl> node) override;

    virtual bool visitExpNode(std::shared_ptr<ASTExpNode> node) override;

    virtual bool visitExpSeqNode(std::shared_ptr<ASTExpSeqNode> node) override;

    virtual bool visitAllocArrayExp(std::shared_ptr<ASTAllocArrayExp> node) override;

    virtual bool visitBinopExp(std::shared_ptr<ASTBinopExp> node) override;

    virtual bool visitBooleanExp(std::shared_ptr<ASTBooleanExp> node) override;

    virtual bool visitCallExp(std::shared_ptr<ASTCallExp> node) override;

    virtual bool visitIdentifierExp(std::shared_ptr<ASTIdentifierExp> node) override;

    virtual bool visitIndexExp(std::shared_ptr<ASTIndexExp> node) override;

    virtual bool visitIntegerExp(std::shared_ptr<ASTIntegerExp> node) override;

    virtual bool visitUnopExp(std::shared_ptr<ASTUnopExp> node) override;

    virtual bool visitStmtNode(std::shared_ptr<ASTStmtNode> node) override;

    virtual bool visitStmtSeqNode(std::shared_ptr<ASTStmtSeqNode> node) override;

    virtual bool visitAssignStmt(std::shared_ptr<ASTAssignStmt> node) override;

    virtual bool visitExprStmt(std::shared_ptr<ASTExprStmt> node) override;

    virtual bool visitIfStmt(std::shared_ptr<ASTIfStmt> node) override;

    virtual bool visitReturnStmt(std::shared_ptr<ASTReturnStmt> node) override;

    virtual bool visitScopeStmt(std::shared_ptr<ASTScopeStmt> node) override;

    virtual bool visitVarDeclStmt(std::shared_ptr<ASTVarDeclStmt> node) override;

    virtual bool visitWhileStmt(std::shared_ptr<ASTWhileStmt> node) override;

    virtual bool visitArgNode(std::shared_ptr<ASTArgNode> node) override;

    virtual bool visitArgSeqNode(std::shared_ptr<ASTArgSeqNode> node) override;

    virtual bool visitTypeNode(std::shared_ptr<ASTTypeNode> node) override;

    virtual bool visitArrType(std::shared_ptr<ASTArrType> node) override;

    virtual bool visitBooleanType(std::shared_ptr<ASTBooleanType> node) override;

    virtual bool visitFunType(std::shared_ptr<ASTFunType> node) override;

    virtual bool visitIdType(std::shared_ptr<ASTIdType> node) override;

    virtual bool visitIntegerType(std::shared_ptr<ASTIntegerType> node) override;

    virtual bool visitPtrType(std::shared_ptr<ASTPtrType> node) override;

    virtual bool visitVoidType(std::shared_ptr<ASTVoidType> node) override;

};

#endif
