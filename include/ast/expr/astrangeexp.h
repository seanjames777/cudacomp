/**
 * @file astrangeexp.h
 *
 * @brief Abstract syntax tree range expression node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTRANGEEXP_H
#define __ASTRANGEEXP_H

#include <defs.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Range expression AST node
 */
class ASTRangeExp : public ASTExpNode {
private:

    std::shared_ptr<ASTExpNode> min;
    std::shared_ptr<ASTExpNode> max;

public:

    /**
     * @brief Constructor
     *
     * @param[in] e1 Range min, inclusive
     * @param[in] e2 Range max, exclusive
     */
    ASTRangeExp(std::shared_ptr<ASTExpNode> min, std::shared_ptr<ASTExpNode> max);

    /**
     * @brief Destructor
     */
    ~ASTRangeExp();

    /**
     * @brief Get minimum value, inclusive
     */
    std::shared_ptr<ASTExpNode> getMin();

    /**
     * @brief Get maximum value, exclusive
     */
    std::shared_ptr<ASTExpNode> getMax();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
