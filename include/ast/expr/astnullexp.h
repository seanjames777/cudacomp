/**
 * @file astnullexp.h
 *
 * @brief Abstract syntax tree null pointer node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTNULLEXP_H
#define __ASTNULLEXP_H

#include <defs.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Null pointer AST node
 */
class ASTNullExp : public ASTExpNode {
public:

    /**
     * @brief Constructor
     *
     * @param[in] value Integer constant value
     */
    ASTNullExp();

    /**
     * @brief Destructor
     */
    ~ASTNullExp();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
