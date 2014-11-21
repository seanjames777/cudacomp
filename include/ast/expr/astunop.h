/**
 * @file astunop.h
 *
 * @brief Abstract syntax tree unary operation node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTUNOP_H
#define __ASTUNOP_H

#include <defs.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Unary operator expression AST node
 */
class ASTUnop : public ASTExpNode {
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
    ASTExpNode *exp;

public:

    /**
     * @brief Constructor
     *
     * @param[in] op  Unary operator
     * @param[in] exp Right-hand expression
     */
    ASTUnop(enum op op, ASTExpNode *exp);

    /**
     * @brief Destructor
     */
    ~ASTUnop();

    /**
     * @brief Get unary operator
     */
    enum op getOp();

    /**
     * @brief Get right-hand expression
     */
    ASTExpNode *getExp();

    /**
     * @copydoc ASTNode::print
     */
    void print(std::ostream & ss) override;

};

#endif
