/**
 * @file astscopestmt.h
 *
 * @brief Abstract syntax tree scope node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTSCOPESTMT_H
#define __ASTSCOPESTMT_H

#include <ast/stmt/aststmtnode.h>

/**
 * @brief Scope statement AST node. Any variables declared within the scope will
 * be confined to the subtree in the scope.
 */
class ASTScopeStmt : public ASTStmtNode {
private:

    ASTStmtSeqNode *body;

public:

    /**
     * @brief Constructor
     *
     * @param[in] body Scope body statement sequence
     */
    ASTScopeStmt(ASTStmtSeqNode *body);

    /**
     * @brief Destructor
     */
    ~ASTScopeStmt();

    /**
     * @brief Get the scope body statement sequence
     */
    ASTStmtSeqNode *getBody();

    /**
     * @copydoc ASTNode::print()
     */
    void print(std::ostream & ss) override;

};

#endif
