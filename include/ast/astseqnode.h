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

#include <ast/astnode.h>

template<typename T>
class ASTSeqNode : public ASTNode {
private:

    T *head;
    ASTSeqNode<T> *tail;

public:

    ASTSeqNode(T *head, ASTSeqNode<T> *tail)
        : head(head),
          tail(tail)
    {
    }

    ~ASTSeqNode() {
        delete head;

        if (tail)
            delete tail;
    }

    T *getHead() {
        return head;
    }

    ASTSeqNode<T> *getTail() {
        return tail;
    }

    void print(std::ostream & ss) override {
        ss << "seq(";
        getHead()->print(ss);
        ss << "," << std::endl;

        if (getTail())
            getTail()->print(ss);
        else
            ss << "null";

        ss << ")";
    }

};

#endif
