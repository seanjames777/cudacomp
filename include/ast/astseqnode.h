/**
 * @file astseqnode.h
 *
 * @brief Abstract syntax tree sequence node.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTSEQNODE_H
#define __ASTSEQNODE_H

#include <ast/astnode.h>

/**
 * @brief Abstract base class of all "sequence" nodes. These are nodes which have
 * a head and a tail, where the head is the first element of a sequence and the
 * tail is another sequence node representing the rest of the sequence, or
 * null to indicate the end of the sequence.
 *
 * @tparam T Type of "head" element
 */
template<typename T>
class ASTSeqNode : public ASTNode {
private:

    T *head;
    ASTSeqNode<T> *tail;

public:

    /**
     * @brief Constructor
     *
     * @param[in] head First element of the sequence
     * @param[in] tail The rest of the sequence
     */
    ASTSeqNode(T *head, ASTSeqNode<T> *tail)
        : head(head),
          tail(tail)
    {
    }

    /**
     * @brief Destructor
     */
    ~ASTSeqNode() {
        delete head;

        if (tail)
            delete tail;
    }

    /**
     * @brief Get the first element of the sequence
     */
    T *getHead() {
        return head;
    }

    /**
     * @brief Get the rest of the sequence
     */
    ASTSeqNode<T> *getTail() {
        return tail;
    }

    /**
     * @brief Set the head pointer
     */
    void setHead(T *head) {
        this->head = head;
    }

    /**
     * @brief Set the tail pointer
     */
    void setTail(ASTSeqNode<T> *tail) {
        this->tail = tail;
    }

    /**
     * @copydoc ASTNode::print
     */
    virtual void print(std::ostream & ss) override {
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
