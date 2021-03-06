/**
 * @file astderefexp.h
 *
 * @brief Abstract syntax tree dereference operation node
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#ifndef __ASTDEREFEXP_H
#define __ASTDEREFEXP_H

#include <defs.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Dereference operator expression AST node
 */
class ASTDerefExp : public ASTExpNode {
private:

    std::shared_ptr<ASTExpNode> exp;
    bool isParenthesized;

public:

    /**
     * @brief Constructor
     *
     * @param[in] exp Right-hand expression
     */
    ASTDerefExp(std::shared_ptr<ASTExpNode> exp);

    /**
     * @brief Destructor
     */
    ~ASTDerefExp();

    /**
     * @brief Get right-hand expression
     */
    std::shared_ptr<ASTExpNode> getExp();

    /**
     * @brief Gets if this dereference was bound by parens in parsing
     */
    bool getParenthesization();

    /**
     * @brief Sets if this dereference was bound by parens in parsing.
     */
    void setParenthesization(bool isParenthesized);

    /**
     * @copydoc ASTNode::print
     */
    void print(std::ostream & ss) override;

};

#endif
