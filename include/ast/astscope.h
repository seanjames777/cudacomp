/**
 * @file astscope.h
 *
 * @brief Abstract syntax tree scope node. Confines variable declarations to a
 * subtree.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTSCOPE_H
#define __ASTSCOPE_H

#include <ast/aststmtnode.h>

class ASTScope : public ASTStmtNode {
private:

    ASTStmtNode *body;

public:

    ASTScope(ASTStmtNode *body);

    ~ASTScope();

    ASTStmtNode *getBody();

    void print(std::ostream & ss) override;

};

#endif
