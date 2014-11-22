/**
 * @file astexprstmt.h
 *
 * @brief Abstract syntax tree expression statement node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTEXPRSTMT_H
#define __ASTEXPRSTMT_H

#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Expression statement AST node. This is a statement that wraps an
 * expression, which is evaluated for its effects without assigning the result
 * to a variable. For example, void function calls use this node.
 */
class ASTExprStmt : public ASTStmtNode {
private:

    std::shared_ptr<ASTExpNode> exp;

public:

    /**
     * @brief Constructor
     *
     * @param[in] exp Expression to evaluate
     */
    ASTExprStmt(std::shared_ptr<ASTExpNode> exp);

    /**
     * @brief Destructor
     */
    ~ASTExprStmt();

    /**
     * @brief Get expression to evaluate
     */
    std::shared_ptr<ASTExpNode> getExp();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
