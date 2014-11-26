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

    void enter(std::shared_ptr<ASTNode> node);

    void leave();

    void color(int color);

    void uncolor();

public:

    ASTPrint(std::ostream & out, bool colorize);

    void run(std::shared_ptr<ASTDeclSeqNode> ast);

    virtual void visitNode(std::shared_ptr<ASTNode> node) override;

};

#endif
