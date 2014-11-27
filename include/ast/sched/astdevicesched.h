/**
 * @file astdevicesched.h
 *
 * @brief GPU scheduling node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTDEVICESCHED_H
#define __ASTDEVICESCHED_H

#include <ast/sched/astschednode.h>

/**
 * @brief Scheduling node which indicates that an expression, which must be a
 * function call for now, should run on the GPU.
 */
class ASTDeviceSched : public ASTSchedNode {
private:

    std::shared_ptr<ASTExpNode> exp;

public:

    /**
     * @brief Constructor
     */
    ASTDeviceSched(std::shared_ptr<ASTExpNode> exp);

    /**
     * @brief Get the expression to run
     */
    std::shared_ptr<ASTExpNode> getExp();

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & out) override;

};

#endif
