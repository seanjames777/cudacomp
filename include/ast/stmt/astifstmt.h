/**
 * @file astifstmt.h
 *
 * @brief Abstract syntax tree 'if' statement node.
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTIFSTMTNODE_H
#define __ASTIFSTMTNODE_H

#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astexpnode.h>

/**
 * @brief If statement AST node. Conditionally evaluates one branch.
 */
class ASTIfStmt : public ASTStmtNode {
private:

    std::shared_ptr<ASTExpNode> cond;
    std::shared_ptr<ASTStmtSeqNode> trueStmt;
    std::shared_ptr<ASTStmtSeqNode> falseStmt;

public:

    /**
     * Constructor
     *
     * @param[in] cond      Branch condition
     * @param[in] trueStmt  Statement sequence to evaluate when condition is true
     * @param[in] falseStmt Statement sequence to evaluate when condition is
     *                      false. May be null.
     */
    ASTIfStmt(std::shared_ptr<ASTExpNode> cond, std::shared_ptr<ASTStmtSeqNode> trueStmt, std::shared_ptr<ASTStmtSeqNode> falseStmt);

    /**
     * @brief Destructor
     */
    ~ASTIfStmt();

    /**
     * @brief Get expression to branch on
     */
    std::shared_ptr<ASTExpNode> getCond();

    /**
     * @brief Get statement sequence to evaluate when condition is true
     */
    std::shared_ptr<ASTStmtSeqNode> getTrueStmt();

    /**
     * @brief Get statement sequence to evaluate when condition is false. May
     * be null.
     */
    std::shared_ptr<ASTStmtSeqNode> getFalseStmt();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
