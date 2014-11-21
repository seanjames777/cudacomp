/**
 * @file astidentifierexp.h
 *
 * @brief Abstract syntax tree identifier node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTIDENTIFIEREXP_H
#define __ASTIDENTIFIEREXP_H

#include <defs.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief AST identifier expression nodes. Refers to a local variable in a
 * function body.
 */
class ASTIdentifierExp : public ASTExpNode {
private:

    std::string value;

public:

    /**
     * @brief Constructor
     *
     * @param[in] value Local variable name
     */
    ASTIdentifierExp(std::string value);

    /**
     * @brief Destructor
     */
    ~ASTIdentifierExp();

    /**
     * @brief Get the referenced variable name
     */
    std::string getId();

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
