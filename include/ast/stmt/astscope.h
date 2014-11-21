/**
 * @file astscope.h
 *
 * @brief Abstract syntax tree scope node
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#ifndef __ASTSCOPE_H
#define __ASTSCOPE_H

#include <ast/stmt/aststmtnode.h>

/**
 * @brief Scope statement AST node. Any variables declared within the scope will
 * be confined to the subtree in the scope.
 */
class ASTScope : public ASTStmtNode {
private:

    ASTStmtSeqNode *body;

public:

    /**
     * @brief Constructor
     *
     * @param[in] body Scope body statement sequence
     */
    ASTScope(ASTStmtSeqNode *body);

    /**
     * @brief Destructor
     */
    ~ASTScope();

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
