/**
 * @file astinteger.h
 *
 * @brief Abstract syntax tree integer node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTINTEGER_H
#define __ASTINTEGER_H

#include <defs.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Integer constant expression AST node
 */
class ASTInteger : public ASTExpNode {
private:

    int value;

public:

    /**
     * @brief Constructor
     *
     * @param[in] value Integer constant value
     */
    ASTInteger(int value);

    /**
     * @brief Destructor
     */
    ~ASTInteger();

    /**
     * @brief Get integer value
     */
    int getValue();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
