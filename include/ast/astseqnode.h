/**
 * @file astseqnode.h
 *
 * @brief Abstract syntax tree sequence statement node. Evaluates its head and
 * then its tail, which may be empty (NULL).
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTSEQNODE_H
#define __ASTSEQNODE_H

#include <ast/aststmtnode.h>

class ASTSeqNode : public ASTStmtNode {
private:

    ASTStmtNode *head;
    ASTSeqNode *tail;

public:

    ASTSeqNode(ASTStmtNode *head, ASTSeqNode *tail);

    ~ASTSeqNode();

    ASTStmtNode *getHead();

    ASTSeqNode *getTail();

    void print(std::ostream & ss) override;

};

#endif
