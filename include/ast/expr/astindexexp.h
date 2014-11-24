/**
 * @file astindexexp.h
 *
 * @brief Array subscripting expression or lvalue AST node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTINDEXEXP_H
#define __ASTINDEXEXP_H

#include <ast/expr/astexpnode.h>

/**
 * @brief Array subcripting expression/lvalue AST node
 */
class ASTIndexExp : public ASTExpNode {
private:

    std::shared_ptr<ASTExpNode> lvalue;
    std::shared_ptr<ASTExpNode> subscript;

public:

    /**
     * @brief Constructor
     *
     * @param[in] lvalue    LValue to assign to
     * @param[in] subscript Array index to assign to
     */
    ASTIndexExp(std::shared_ptr<ASTExpNode> lvalue, std::shared_ptr<ASTExpNode> subscript);

    /**
     * @brief Get the lvalue being indexed into
     */
    std::shared_ptr<ASTExpNode> getLValue();

    /**
     * @brief Get the subscript/index expression
     */
    std::shared_ptr<ASTExpNode> getSubscript();

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
