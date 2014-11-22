/**
 * @file astunopexp.h
 *
 * @brief Abstract syntax tree unary operation node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTUNOPEXP_H
#define __ASTUNOPEXP_H

#include <defs.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Unary operator expression AST node
 */
class ASTUnopExp : public ASTExpNode {
public:

    /**
     * @brief Unary operators
     */
    enum op {
        NOT,   //!< Boolean NOT
        BNOT,  //!< Bitwise NOT
        NEG    //!< Numeric negation
    };

private:

    enum op op;
    std::shared_ptr<ASTExpNode> exp;

public:

    /**
     * @brief Constructor
     *
     * @param[in] op  Unary operator
     * @param[in] exp Right-hand expression
     */
    ASTUnopExp(enum op op, std::shared_ptr<ASTExpNode> exp);

    /**
     * @brief Destructor
     */
    ~ASTUnopExp();

    /**
     * @brief Get unary operator
     */
    enum op getOp();

    /**
     * @brief Get right-hand expression
     */
    std::shared_ptr<ASTExpNode> getExp();

    /**
     * @copydoc ASTNode::print
     */
    void print(std::ostream & ss) override;

};

#endif
