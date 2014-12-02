/**
 * @file astforstmt.h
 *
 * @brief Abstract syntax tree 'for' statement node
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#ifndef __ASTFORSTMT_H
#define __ASTFORSTMT_H

#include <ast/stmt/aststmtnode.h>
#include <ast/expr/astexpnode.h>

class ASTForStmt : public ASTStmtNode {
private:

    std::shared_ptr<ASTStmtNode> init;
    std::shared_ptr<ASTExpNode> cond;
    std::shared_ptr<ASTStmtNode> iter;
    std::shared_ptr<ASTStmtSeqNode> bodyStmt;

public:

    /**
     * @brief Constructor
     *
     * @param[in] init     Loop initializing statement
     * @param[in] cond     Loop condition
     * @param[in] iter     Loop iteration statement
     * @param[in] bodyStmt Statement sequence to evaluate in a loop while the condition is true
     */
    ASTForStmt(std::shared_ptr<ASTStmtNode> init, std::shared_ptr<ASTExpNode> cond,
        std::shared_ptr<ASTStmtNode> iter, std::shared_ptr<ASTStmtSeqNode> bodyStmt);

    /**
     * @brief Destructor
     */
    ~ASTForStmt();

    /**
     * @brief Get the statement for the loop's initializing statement
     */
    std::shared_ptr<ASTStmtNode> getInit();

    /**
     * @brief Get expression for loop's condition
     */
    std::shared_ptr<ASTExpNode> getCond();

    /**
     * @brief Get the statement for the loop's iteration statement
     */
    std::shared_ptr<ASTStmtNode> getIter();

    /**
     * @brief Get statement sequence to evaluate in a loop while the condition is true
     */
    std::shared_ptr<ASTStmtSeqNode> getBody();

    /**
     * @copydoc ASTNode::print()
     */
    virtual void print(std::ostream & ss) override;

};

#endif
