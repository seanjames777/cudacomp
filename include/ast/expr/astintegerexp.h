/**
 * @file astintegerexp.h
 *
 * @brief Abstract syntax tree integer node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTINTEGEREXP_H
#define __ASTINTEGEREXP_H

#include <defs.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Integer constant expression AST node
 */
class ASTIntegerExp : public ASTExpNode {
private:

    int value;

public:

    /**
     * @brief Constructor
     *
     * @param[in] value Integer constant value
     */
    ASTIntegerExp(int value);

    /**
     * @brief Destructor
     */
    ~ASTIntegerExp();

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
