/**
 * @file astfloatexp.h
 *
 * @brief Abstract syntax tree float node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTFLOATEXP_H
#define __ASTFLOATEXP_H

#include <defs.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Float constant expression AST node
 */
class ASTFloatExp : public ASTExpNode {
private:

    float value;

public:

    /**
     * @brief Constructor
     *
     * @param[in] value Float constant value
     */
    ASTFloatExp(float value);

    /**
     * @brief Destructor
     */
    ~ASTFloatExp();

    /**
     * @brief Get float value
     */
    float getValue();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
