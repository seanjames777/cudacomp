/**
 * @file astbooleanexp.h
 *
 * @brief Abstract syntax tree boolean node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTBOOLEANEXP_H
#define __ASTBOOLEANEXP_H

#include <defs.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Boolean constant expression AST node
 */
class ASTBooleanExp : public ASTExpNode {
private:

    bool value;

public:

    /**
     * @brief Constructor
     *
     * @param[in] value Boolean constant value
     */
    ASTBooleanExp(bool value);

    /**
     * @brief Destructor
     */
    ~ASTBooleanExp();

    /**
     * @brief Get boolean value
     */
    bool getValue();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
