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

#include <ast/stmt/aststmtnode.h>

class ASTScope : public ASTStmtNode {
private:

    ASTStmtSeqNode *body;

public:

    ASTScope(ASTStmtSeqNode *body);

    ~ASTScope();

    ASTStmtSeqNode *getBody();

    void print(std::ostream & ss) override;

};

#endif
