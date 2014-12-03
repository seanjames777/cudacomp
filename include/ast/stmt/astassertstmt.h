/**
 * @file astassertstmt.h
 *
 * @brief Abstract syntax tree 'assert' statement node.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTASSERTSTMTNODE_H
#define __ASTASSERTSTMTNODE_H

#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief Assert statement AST node. Checks that a condition expression evaluates to true.
 */
class ASTAssertStmt : public ASTStmtNode {
private:

    std::shared_ptr<ASTExpNode> cond;

public:

    /**
     * Constructor
     *
     * @param[in] cond Assertion condition
     */
    ASTAssertStmt(std::shared_ptr<ASTExpNode> cond);

    /**
     * @brief Destructor
     */
    ~ASTAssertStmt();

    /**
     * @brief Get expression to branch on
     */
    std::shared_ptr<ASTExpNode> getCond();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
