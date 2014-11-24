/**
 * @file astassignstmt.h
 *
 * @brief Abstract syntax tree assignment statement node.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTASSIGNSTMT_H
#define __ASTASSIGNSTMT_H

#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Assignment statement AST node. Assigns a new value to an lvalue, where
 * an lvalue may be a variable, an array element, etc.
 */
class ASTAssignStmt : public ASTStmtNode {
private:

    std::shared_ptr<ASTExpNode> lvalue;
    std::shared_ptr<ASTExpNode> exp;

public:

    /**
     * @brief Constructor
     *
     * @param[in] id  Variable name
     * @param[in] exp New value expression
     */
    ASTAssignStmt(std::shared_ptr<ASTExpNode> lvalue, std::shared_ptr<ASTExpNode> exp);

    /**
     * @brief Destructor
     */
    ~ASTAssignStmt();

    /**
     * @brief Get lvalue to assign to
     */
    std::shared_ptr<ASTExpNode> getLValue();

    /**
     * @brief Get expression to assign value from
     */
    std::shared_ptr<ASTExpNode> getExp();

    /**
     * @coypdoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
