/**
 * @file astboolean.h
 *
 * @brief Abstract syntax tree boolean node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTBOOLEAN_H
#define __ASTBOOLEAN_H

#include <defs.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Boolean constant expression AST node
 */
class ASTBoolean : public ASTExpNode {
private:

    bool value;

public:

    /**
     * @brief Constructor
     *
     * @param[in] value Boolean constant value
     */
    ASTBoolean(bool value);

    /**
     * @brief Destructor
     */
    ~ASTBoolean();

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
