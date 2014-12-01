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

    static std::shared_ptr<ASTDeviceSched> instance;

public:

    /**
     * @brief Constructor
     */
    ASTDeviceSched();

    /**
     * @brief Get singleton instance
     */
    static std::shared_ptr<ASTDeviceSched> get();

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & out) override;

};

#endif
