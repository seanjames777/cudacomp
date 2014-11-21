/**
 * @file astidentifier.h
 *
 * @brief Abstract syntax tree identifier node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTIDENTIFIER_H
#define __ASTIDENTIFIER_H

#include <defs.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief AST identifier expression nodes. Refers to a local variable in a
 * function body.
 */
class ASTIdentifier : public ASTExpNode {
private:

    std::string value;

public:

    /**
     * @brief Constructor
     *
     * @param[in] value Local variable name
     */
    ASTIdentifier(std::string value);

    /**
     * @brief Destructor
     */
    ~ASTIdentifier();

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
