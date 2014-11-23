/**
 * @file astwhilestmt.h
 *
 * @brief Abstract syntax tree 'while' statement node
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#ifndef __ASTWHILESTMT_H
#define __ASTWHILESTMT_H

#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astexpnode.h>

class ASTWhileStmt : public ASTStmtNode {
private:

    std::shared_ptr<ASTExpNode> cond;
    std::shared_ptr<ASTStmtSeqNode> bodyStmt;

public:

    /**
     * @brief Constructor
     *
     * @param[in] cond     Loop condition
     * @param[in] bodyStmt Statement sequence to evaluate in a loop while the condition is true
     */
    ASTWhileStmt(std::shared_ptr<ASTExpNode> cond, std::shared_ptr<ASTStmtSeqNode> bodyStmt);

    /**
     * @brief Destructor
     */
    ~ASTWhileStmt();

    /**
     * @brief Get expression for loop's condition
     */
    std::shared_ptr<ASTExpNode> getCond();

    /**
     * @brief Get statement sequence to evaluate in a loop while the condition is true
     */
    std::shared_ptr<ASTStmtSeqNode> getBodyStmt();

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
