/**
 * @file astternopexp.h
 *
 * @brief Abstract syntax tree ternary operator node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTTERNOPEXP_H
#define __ASTTERNOPEXP_H

#include <defs.h>
#include <ast/expr/astexpnode.h>
#include <ast/type/asttypenode.h>

/**
 * @brief Ternary operator expression AST node
 */
class ASTTernopExp : public ASTExpNode {
private:

    std::shared_ptr<ASTExpNode> cond;
    std::shared_ptr<ASTExpNode> eTrue;
    std::shared_ptr<ASTExpNode> eFalse;
    std::shared_ptr<ASTTypeNode> type;

public:

    /**
     * @brief Constructor
     *
     * @param[in] cond   Condition expression
     * @param[in] eTrue  True expression
     * @param[in] eFalse False expression
     */
    ASTTernopExp(
        std::shared_ptr<ASTExpNode> cond,
        std::shared_ptr<ASTExpNode> eTrue,
        std::shared_ptr<ASTExpNode> eFalse);

    /**
     * @brief Destructor
     */
    ~ASTTernopExp();

    /**
     * @brief Get condition expression
     */
    std::shared_ptr<ASTExpNode> getCond();

    /**
     * @brief Get left-hand expression
     */
    std::shared_ptr<ASTExpNode> getTrueExp();

    /**
     * @brief Get right-hand expression
     */
    std::shared_ptr<ASTExpNode> getFalseExp();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
